#include "../h/Layer.h"
#include "../h/LayerException.h"


Layer::Layer(int w, int h, unsigned int opac, bool active) :width(w), height(h), opacity(opac), isActive(active) {
	pixels.assign(width * height, Pixel());                                                               // make white layer with alpha 0x00
}

Layer::Layer(const std::vector<int>& pix, int w, int h, unsigned int opacity_, bool active_) : width(w), height(h), opacity(opacity_), isActive(active_) {
	for (int it : pix)
		pixels.push_back(Pixel(it & 0xff, (it >> 8) & 0xff, (it >> 16) & 0xff, (it >> 24) & 0xff));
}

Layer::Layer(int w, int h, unsigned int opac, bool active, int a) : width(w), height(h), opacity(opac), isActive(active) {
	pixels.assign(width * height, Pixel(a));
}

void Layer::copy(const Layer& l) {
	for (const Pixel& p : l.pixels)
		pixels.push_back(Pixel(p.getR(), p.getG(), p.getB(), p.getA()));
	width = l.width;
	height = l.height;
	opacity = l.opacity;
	isActive = l.isActive;
}

Layer::~Layer() {
	pixels.clear(); //destroy all objects in vector
	std::vector<Pixel>().swap(pixels);
}

void Layer::changeOpacity(unsigned int new_opacity) {
	if (opacity < 0 || opacity > 100) throw LayerInvalidInput();
	opacity = new_opacity;
}

void Layer::changeStatus(bool active) {
	isActive = active;
}

void Layer::addPadding(int w, int h) {
	if (width < w) 
		for (int i = 0; i < height; i++)   //insert transparent pixels to the right
			for (int j = width; j < w; j++)
				pixels.emplace(pixels.begin() + j + w * i, Pixel());
	width = (width > w) ? width : w;
	if (height < h)
		for (int i = height; i < h; i++)  //insert transparent pixels to the bottom
			for (int j = 0; j < width; j++)
				pixels.push_back(Pixel());
	height = (h > height) ? h : height;
}

std::vector<int> Layer::layerToInt() const { // for operation
	std::vector<int> result;
	for (auto it : pixels) {
		result.push_back(it.getR());
		result.push_back(it.getG());
		result.push_back(it.getB());
	}
	return result;
}


std::vector<int> Layer::layerToChar() const {   // when write to file
	std::vector<int> result;
	auto lambda = [](int x) { if (x > 255) return 255; else return x < 0 ? 0 : x; };
	for (auto it : pixels) 
		result.push_back((lambda(it.getR()) & 0x000000ff) | ((lambda(it.getG()) << 8) & 0x0000ff00) | ((lambda(it.getB()) << 16) & 0x00ff0000) | ((lambda(it.getA()) << 24) & 0xff000000));
	return result;
} 

void Layer::intToLayer(std::vector<int>& pix) {                                          // for operation
	auto lambda = [](int x) { if (x > 255) return 255; else return (x < 0) ? 0 : x; };
	int i = 0;
	for (auto& it : pixels) {
		it.set(lambda(pix[i]), lambda(pix[i + 1]), lambda(pix[i + 2]));
		i += 3;
	}	
}

void Layer::merge(Layer* l) {
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			Pixel& p0 = l->pixels[i * width + j];
			Pixel& p1 = pixels[i * width + j];
			double a0 = (double)p0.getA() * (double)l->getOpacity() / 100.0 / 255.0;
			double a1 = (double)p1.getA() * (double)getOpacity() / 100.0 / 255.0;
			double a = a0 + (1 - a0) * a1;
			if (a != 0) {
				p0.setR((int)(p0.getR() * a0 / a + p1.getR() * (1 - a0) * a1 / a));
				p0.setG((int)(p0.getG() * a0 / a + p1.getG() * (1 - a0) * a1 / a));
				p0.setB((int)(p0.getB() * a0 / a + p1.getB() * (1 - a0) * a1 / a));
			}
			else {
				p0.setR(0); p0.setG(0); p0.setB(0);
			}
			p0.setA((int)(a * 255));
		}
}


void Layer::fillPixels(const std::set<std::pair<int, int>>& selected, std::tuple<int, int, int>& t) {
	for (auto it : selected)
		if (it.first < width && it.first > 0 && it.second < height && it.second > 0)
			pixels[it.second * width + it.first].set(std::get<0>(t), std::get<1>(t), std::get<2>(t));
}
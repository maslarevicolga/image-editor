#include "../h/ImageLayers.h"
#include "../h/LayerException.h"


Layer* ImageLayers::getLayer(int index) const {
	if (index >= layers.size() || index < 0) throw LayerOutOfBounds();
	return layers[index];
}


void ImageLayers::swapLayers(int index1, int index2) {
	if (index1 >= layers.size() || index2 >= layers.size() || index1 < 0 || index2 < 0) throw LayerOutOfBounds();
	Layer* tmp = layers[index1];
	layers[index1] = layers[index2];
	layers[index2] = tmp;
	tmp = nullptr;
}

void ImageLayers::removeLayer(int index) {
	if (index >= layers.size() || index < 0) throw LayerOutOfBounds();
	Layer* find = layers.at(index);
	std::vector<Layer*>::iterator it = std::find(layers.begin(), layers.end(), find);
	layers.erase(it);
}

void ImageLayers::push_back(int w, int h, unsigned int opacity, bool active, int alpha) {
	layers.push_back(new Layer(w, h, opacity, active, alpha));
	resize(w, h);
}

void ImageLayers::push_back(int w, int h, unsigned int opacity, bool active) {
	layers.push_back(new Layer(w, h, opacity, active));
	resize(w, h);
}

void ImageLayers::push_back(const std::vector<int>& pixels, int w, int h) {
	layers.push_back(new Layer(pixels, w, h));
	resize(w, h);
}

void ImageLayers::validate(int w, int h, int o) const {
	if (w <= 0 || h <= 0 || o < 0 || o > 100) throw LayerInvalidInput();               //check when user create photo
}

void ImageLayers::resize(int w, int h) {
	if (h > max_height || w > max_width) 
		for (Layer* l : layers) 
			l->addPadding(w, h);
	max_height = (h > max_height) ? h : max_height;
	max_width = (w > max_width) ? w : max_width;
	(*layers.rbegin())->addPadding(max_width, max_height);
}


Layer* ImageLayers::findFirstActive() const {
	for (auto it : layers)
		if (it->getIsActive() == true)
			return it;
	return nullptr;
}

std::vector<int> ImageLayers::merge() const{
	Layer* first = findFirstActive();
	if (first == nullptr) throw ImageEmpty();
	Layer* result = new Layer(first->getWidth(), first->getHeight(), 100);
	for (auto l = std::find(layers.begin(), layers.end(), first); l != layers.end(); l++) {
		if ((*l)->getIsActive() == false) continue;
		(*l)->merge(result);
	}
    return result->layerToChar();
}
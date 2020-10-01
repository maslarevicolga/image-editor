#include "../h/Operation.h"
#include <cmath>
#include <algorithm>
#include <utility>

void Operation::operator()(std::vector<int>& pixels, const std::set<std::pair<int, int>>& selected, int w) {
	if (selected.size() == 0) {
		for (int i = 0; i < pixels.size() / w / 3; i++)
			for (int j = 0; j < w; j++)
				calculate(pixels[i * w * 3 + 3 * j], pixels[i * w * 3 + 3 * j + 1], pixels[i * w * 3 + 3 * j + 2]);
	}else{
		for (auto it : selected) {
			int i = it.second;
			int j = it.first;
			if(i < pixels.size() / w / 3 && j < w)
				calculate(pixels[i * w * 3 + 3 * j], pixels[i * w * 3 + 3 * j + 1], pixels[i * w * 3 + 3 * j + 2]);
		}
	}
}

void Add::calculate(int& r, int& g, int& b) {
	r += param; g += param; b += param;
}

void Sub::calculate(int& r, int& g, int& b) {
	r -= param; g -= param; b -= param;
}

void Sub_Inv::calculate(int& r, int& g, int& b) {
	r = param - r; g = param - g; b = param - b;
}

void Mul::calculate(int& r, int& g, int& b) {
	r *= param; g *= param; b *= param;
}

void Div::calculate(int& r, int& g, int& b) {
	if (param == 0) r = g = b = INT_MAX;
	else {
		r /= param; g /= param; b /= param;
	}
}

void Div_Inv::calculate(int& r, int& g, int& b) {
	r = (r == 0) ? INT_MAX : (param / r);
	g = (g == 0) ? INT_MAX : (param / g);
	b = (b == 0) ? INT_MAX : (param / b);
}

void Pow::calculate(int& r, int& g, int& b) {
	r = (int)std::pow(r, param); g = (int)std::pow(g, param); b = (int)std::pow(b, param);
}

void Log::calculate(int& r, int& g, int& b) {
	r = (int)std::log(r); g = (int)std::log(g); b = (int)std::log(b);
}

void Abs::calculate(int& r, int& g, int& b) {
	r = std::abs(r); g = std::abs(g); b = std::abs(b);
}

void Max::calculate(int& r, int& g, int& b) {
	r = (r > param) ? r : param; g = (g > param) ? g : param; b = (b > param) ? b : param;
}

void Min::calculate(int& r, int& g, int& b) {
	r = (r < param) ? r : param; g = (g < param) ? g : param; b = (b < param) ? b : param;
}

void Grayscale::calculate(int& r, int& g, int& b) {
	double mean = ((double)r + (double)g + (double)b) / 3;
	r = (int)mean; g = (int)mean; b = (int)mean;
}

void Inversion::calculate(int& r, int& g, int& b) {
	r = 255 - r; g = 255 - g; b = 255 - b;
}

void Black_White::calculate(int& r, int& g, int& b) {
	double mean = ((double)r + (double)g + (double)b) / 3;
	r = g = b = (mean < 127) ? 0 : 255;
}

void Median::operator()(std::vector<int>& pixels, const std::set<std::pair<int, int>>& selected, int w) {
	std::vector<int> copy = pixels;
	int h = pixels.size() / (3 * w);

	if (selected.size() == 0) {
		for (int i = 0; i < pixels.size() / w / 3; i++)
			for (int j = 0; j < w; j++)
				calculate(pixels,copy, w, h, i, j);
	}
	else {
		for (auto it : selected) {
			int i = it.second;
			int j = it.first;
			if (i < pixels.size() / w / 3 && j < w)
				calculate(pixels, copy, w, h, i, j);
		}
	}
}

void Median::calculate(std::vector<int>& pixels, std::vector<int>& copy, int w, int h, int i, int j) {
	std::vector<int> n_r, n_g, n_b;
	n_r.push_back(copy[i * w * 3 + 3 * j]); n_g.push_back(copy[i * w * 3 + 3 * j + 1]);  n_b.push_back(copy[i * w * 3 + 3 * j + 2]);
	if (j < w - 1) { n_r.push_back(copy[i * w * 3 + 3 * (j + 1)]); n_g.push_back(copy[i * w * 3 + 3 * (j + 1) + 1]); n_b.push_back(copy[i * w * 3 + 3 * (j + 1) + 2]); }
	if (i > 0) { n_r.push_back(copy[(i - 1) * w * 3 + 3 * j]);  n_g.push_back(copy[(i - 1) * w * 3 + 3 * j + 1]);  n_b.push_back(copy[(i - 1) * w * 3 + 3 * j + 2]); }
	if (j > 0) { n_r.push_back(copy[i * w * 3 + 3 * (j - 1)]); n_g.push_back(copy[i * w * 3 + 3 * (j - 1) + 1]); n_b.push_back(copy[i * w * 3 + 3 * (j - 1) + 2]); }
	if (i < h - 1) { n_r.push_back(copy[(i + 1) * w * 3 + 3 * j]); n_g.push_back(copy[(i + 1) * w * 3 + 3 * j + 1]);  n_b.push_back(copy[(i + 1) * w * 3 + 3 * j + 2]); }
	std::sort(n_r.begin(), n_r.end());
	std::sort(n_g.begin(), n_g.end());
	std::sort(n_b.begin(), n_b.end());
	if (n_r.size() % 2 == 1) {
		pixels[i * w * 3 + 3 * j] = n_r[n_r.size() / 2];
		pixels[i * w * 3 + 3 * j + 1] = n_g[n_g.size() / 2];
		pixels[i * w * 3 + 3 * j + 2] = n_b[n_b.size() / 2];
	}
	else {
		pixels[i * w * 3 + 3 * j] = (n_r[1] + n_r[2]) / 2;
		pixels[i * w * 3 + 3 * j + 1] = (n_g[1] + n_g[2]) / 2;
		pixels[i * w * 3 + 3 * j + 2] = (n_b[1] + n_b[2]) / 2;
	}					
}

void Composite::operator()(std::vector<int>& pixels, const std::set<std::pair<int, int>>& selected, int w) {
	for (auto it : operations)
		(*it)(pixels, selected, w);
}

std::vector<std::string> Composite::saveFunction() const {
	std::vector<std::string> funs;
	for (auto& it : operations)
		funs.push_back(it->toString());
	return funs;
}

Composite::~Composite() {
	for (int i = 0; i < operations.size(); i++) {
		if (operations[i]) {
			delete operations[i];
			operations[i] = nullptr;
		}
	}
}

void Composite::remove(const std::string& name) {
	auto it = operations.begin();
	while (it != operations.end())
		if ((*it)->getName() == (name + "()"))
			it = operations.erase(std::find(operations.begin(), operations.end(), *it));
		else
			it++;
}
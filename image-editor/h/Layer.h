#ifndef _LAYER_H
#define _LAYER_H

#include "Pixel.h"
#include <vector>
#include <string>
#include <set>
#include <tuple>


class Layer {

	std::vector<Pixel> pixels;
	int width, height;
	unsigned int opacity; // 0 -> proziran potpuno; 100 -> potpuno neproziran
	bool isActive;

	
public:

	Layer(int w = 640, int h = 480, unsigned int opacity = 0, bool active = false); //when not reading from image pixels are white and opacity = 0
	Layer(int, int, unsigned int, bool, int);
	Layer(const std::vector<int>&, int w, int h, unsigned int opacity = 100, bool active = true);
	Layer(const Layer& l) { copy(l); }
	~Layer();

	void changeOpacity(unsigned int opacity); //user can change opacity
	void changeStatus(bool active); //active or not

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	bool getIsActive() const { return isActive; }
	unsigned int getOpacity() const { return opacity; }
	Pixel* getPixel(int index) { if (index < width * height && index >= 0) return &pixels[index]; else return nullptr; } // when create set of selected pixels

	void fillPixels(const std::set<std::pair<int, int>>& selected, std::tuple<int, int, int>&); // fill selected pixels with given rgb color

	void addPadding(int w, int h);
	void merge(Layer* l);
	std::vector<int> layerToInt() const;
	std::vector<int> layerToChar() const;
	void intToLayer(std::vector<int>&);
	
private:
	void copy(const Layer&);

};

#endif
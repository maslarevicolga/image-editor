#ifndef _IMAGE_LAYERS_H
#define _IMAGE_LAYERS_H

#include "Layer.h"


class ImageLayers {

	int max_width, max_height;
	std::vector<Layer*> layers;

public:

	Layer* getLayer(int index) const;
	int getDepth()const { return layers.size(); }
	void swapLayers(int, int);
	void removeLayer(int);

	void push_back(int, int, unsigned int, bool, int);
	void push_back(int w = 640, int h = 480, unsigned int opacity = 0, bool active = false);
	void push_back(const std::vector<int>&, int, int);

	std::vector<Layer*>::iterator begin() { return layers.begin(); }
	std::vector<Layer*>::iterator end() { return layers.end(); }
	std::vector<Layer*>::const_iterator begin() const { return layers.begin(); }
	std::vector<Layer*>::const_iterator end() const { return layers.end(); }

	int getWidth() const { return max_width; }
	int getHeight() const { return max_height; }

	std::vector<int> merge() const;

	void validate(int, int, int) const;

private:

	void resize(int w, int h);
	Layer* findFirstActive() const;
};


#endif
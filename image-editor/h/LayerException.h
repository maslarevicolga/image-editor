#ifndef _LAYER_EXCEPTION_H
#define _LAYER_EXCEPTION_H

#include <exception>


class ImageEmpty : public std::exception {
public:
	const char* what() const override {
		return "Error: Image hasn't active layers! \n";
	}
};

class LayerInvalidInput : public std::exception {
public:
	const char* what() const override {
		return "Error: Layer parameters are not allowed! \n";
	}
};

class LayerOutOfBounds : public std::exception {
public:
	const char* what() const override {
		return "Error: The image has fewer layers than the given number! \n";
	}
};

#endif
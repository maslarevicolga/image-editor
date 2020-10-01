#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include <iostream>
#include "SelectionException.h"

class Rectangle_ {
	int x, y;
	int w, h;

public:

	Rectangle_(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_){}

	int getX() const { return x; }
	int getY() const { return y; }
	int getW() const { return w; }
	int getH() const { return h; }


	friend std::ostream& operator<<(std::ostream& os, const Rectangle_& r) {
		return os << "(" << r.x << ", " << r.y << ")" << r.w << "x" << r.h;
	}

};



#endif

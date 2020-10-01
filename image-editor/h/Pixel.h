#ifndef _PIXEL_H
#define _PIXEL_H

class Pixel {

	int r;
	int g;
	int b;
	int a; //FF => neprozirna 00 -> crna slika se dobije

public:

	Pixel(int r_, int g_, int b_, int a_ = 0xff) : r(r_), g(g_), b(b_), a(a_) {} // when create from image
	Pixel(int a_ = 0x00) : r(0x00), g(0x00), b(0x00), a(a_) {}  // when create empty layer

	int getR() const { return r; }
	int getG() const { return g; }
	int getB() const { return b; }
	int getA() const { return a; }

	void setA(int v) { a = v; }
	void setR(int v) { r = v; }
	void setG(int v) { g = v; }
	void setB(int v) { b = v; }
	void set(int r_, int g_, int b_) { r = r_; g = g_; b = b_; }
};



#endif
#ifndef _FORMATER_EXCEPTION_H
#define _FORMATER_EXCEPTION_H

#include <exception>

class OpenFileException : public std::exception {
public:
	const char* what() const override {
		return "Error: Can't open file!\nFile doesn't exist or hasn't .pam or .bmp extension!\n";
	}
};


class InvalidFormatBMP : public std::exception {
public:
	const char* what() const override {
		return "Error: BMP format isn't supported! Height can't be negative and only 32-bit and 24-bit pixels are supported! \n";
	}
};

class InvalidFormatPAM : public std::exception {
public:
	const char* what() const override {
		return "Error: PAM format isn't supported! Tupltype must be RGB or RGB_ALPHA with MAX_VAL 255!\n";
	}
};

#endif
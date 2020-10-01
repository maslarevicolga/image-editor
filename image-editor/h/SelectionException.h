#ifndef _SELECTION_EXCEPTION_H
#define _SELECTION_EXCEPTION_H

#include <exception>

class SelectionNotFound : public std::exception {
public:
	const char* what() const override {
		return "Error: Invalid selection name! \n";
	}
};


class InvalidDimension : public std::exception {
public:
	const char* what() const override {
		return "Error: Invalid dimension! \n";
	}
};


class SelectionExist : public std::exception {
public:
	const char* what() const override {
		return "Error: Selection with that name already exist! \n";
	}
};

#endif
#ifndef _XML_EXCEPTION_H
#define _XML_EXCEPTION_H


#include <exception>

class FileNotFound : public std::exception {
public:
	const char* what() const override {
		return "Error: File doesn't exist! \n";
	}
};

class FileNotFun : public std::exception {
public:
	const char* what() const override {
		return "Error: File isn't .fun! \n";
	}
};

class DirNotFound : public std::exception {
public:
	const char* what() const override {
		return "Error: Path wasn't found!\n";
	}
};

class DirExists : public std::exception {
public:
	const char* what() const override {
		return "Error: Directory with given name already exist!\n";
	}
};


#endif
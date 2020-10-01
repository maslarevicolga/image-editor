#ifndef _OPERATION_EXCEPTION_H
#define _OPERATION_EXCEPTION_H

#include <exception>


class InvalidOperationException : public std::exception {
public:
	const char* what() const override {
		return "Error: Operation doesn't exist! \n";
	}
};

class OperationExistException : public std::exception {
public:
	const char* what() const override {
		return "Error: Operation exist! \n";
	}
};


#endif

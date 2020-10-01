#ifndef _MENU_H_
#define _MENU_H_

#include <iostream>
#include <string>
#include "Image.h"

class InvalidMenuOption : public std::exception {
public:
	const char* what() const override {
		return "Error: Invalid menu option\n";
	}
};

class InvalidInput : public std::exception {
public:
	const char* what() const override {
		return "Error: Invalid input! Check format!\n";
	}
};



class Menu {

public:
	virtual void print_menu();

	void read_menu_option(std::istream& is = std::cin);

	virtual Menu* execute_option(Image& image, std::istream& is = std::cin);

	bool is_the_end() { return is_finished; }

private:

	enum MenuOptions {
		EXIT,
		LAYER, 
		SELECTION, 
		OPERATION, 
		APPLY_COMPOSITE,
		EXPORT_PAM_BMP,
		LOAD_PROJECT,
		SAVE_PROJECT,
		DRAW_ON_CONSOLE

	};
	bool is_finished;
	bool is_exported;

protected:
	MenuOptions menu_option;

	int getIntInput(std::istream&) const;
	char getCharInput(std::istream&) const;

};

#endif
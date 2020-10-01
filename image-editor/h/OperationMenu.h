#ifndef _OPERATION_MENU_H
#define _OPERATION_MENU_H

#include "Menu.h"

class OperationMenu : public Menu {
public:
	void print_menu() override;

	Menu* execute_option(Image& image, std::istream& is = std::cin) override;

private:

	enum MenuOptions {
		GO_BACK,
		CREATE_OPERATION,
		LOAD_OPERATION,
		APPLY_OPERATION,
		LIST_OPERATION
	};

	void parseInput(std::string&, int&) const;

};



#endif
#ifndef  _SELECTION_MENU_H_
#define _SELECTION_MENU_H_


#include "Menu.h"



class SelectionMenu : public Menu {
public:
	void print_menu() override;

	Menu* execute_option(Image& image, std::istream& is = std::cin) override;

private:

	enum MenuOptions {
		GO_BACK,
		CREATE_SELECTION, 
		REMOVE_SELECTION, 
		CHANGE_SELECTION_STATUS, 
		FILL_SELECTION
	};
};


#endif 

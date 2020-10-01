#ifndef LAYER_MENU_H
#define LAYER_MENU_H

#include "Menu.h"

class LayerMenu : public Menu {
public:
	void print_menu() override;

	Menu* execute_option(Image& image, std::istream& is = std::cin);

private:

	enum MenuOptions {
		GO_BACK,
		CREATE_EMPTY_LAYER,
		CREATE_LAYER_WITH_IMAGE,
		CHANGE_LAYER_STATUS,
		CHANGE_LAYER_OPACITY,
		SWAP_LAYERS,
		REMOVE_LAYER
	};

};


#endif // !LAYER_MENU_H

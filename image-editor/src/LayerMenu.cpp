#include "../h/LayerMenu.h"
#include "../h/LayerException.h"
#include "../h/FormaterException.h"

void LayerMenu::print_menu() {
	std::cout
		<< "\n********************************************\n"
		<< GO_BACK << ".  Go back to the previous menu\n"
		<< CREATE_EMPTY_LAYER << ".  Create a blank layer\n"
		<< CREATE_LAYER_WITH_IMAGE << ".  Add an image as a new layer\n"
		<< CHANGE_LAYER_STATUS << ".  Activate layer\n"
		<< CHANGE_LAYER_OPACITY << ".  Change the layer opacity\n"
		<< SWAP_LAYERS << ".  Swap layers\n"
		<< REMOVE_LAYER << ".  Remove layer\n"
		<< "Choose menu option (index of option):\n"
		<< "********************************************\n";
}

Menu* LayerMenu::execute_option(Image& image, std::istream& is) {
	char respond;
	int w, h, opacity, index, index1, index2, alpha;
	bool status;
	std::string file_path;
	std::vector<int> pixels;
	switch (menu_option) {
	
	case CREATE_EMPTY_LAYER:
		try {
			std::cout << "Initial state: wxh -> 640x480!\nDo you want to change the initial state? (y/n) ";
			respond = getCharInput(is);
			if (respond == 'y') {
				std::cout << "Width? "; w = getIntInput(is);
				std::cout << "Height? "; h = getIntInput(is);
				std::cout << "Opacity(0-100)? "; opacity = getIntInput(is);
				std::cout << "Active(1/0)? "; status = getIntInput(is);
				std::cout << "Alpha channel(0-255)? "; alpha = getIntInput(is);
				image.getLayerObject().validate(w, h, opacity);
				image.getLayerObject().push_back(w, h, opacity, status, alpha);
			}
			else
				image.getLayerObject().push_back();
		}
		catch (InvalidInput& e) { std::cout << e.what(); }
		catch (LayerInvalidInput & e) { std::cout << e.what(); }
		return nullptr;
	
	case CREATE_LAYER_WITH_IMAGE:
		std::cout << "File path? ";
		std::getline(std::cin, file_path);
		try {
			image.readFromFile(file_path);
			std::cout << "Inital values: opacity is 100 and layer is active! \nIf want to change choose option CHANGE_OPACITY and CHANGE_STATUS! \n";
		}
		catch (InvalidFormatBMP & e) { std::cout << e.what(); }
		catch (InvalidFormatPAM & e) { std::cout << e.what(); }
		catch (OpenFileException & e) { std::cout << e.what(); }
		return nullptr;

	case CHANGE_LAYER_STATUS:
		try {
			std::cout << "Layer depth? (0-...) "; index = getIntInput(is);
			std::cout << "Active? (1/0) "; status = getIntInput(is);
			image.getLayerObject().getLayer(index)->changeStatus(status);
		}
		catch (InvalidInput & e) { std::cout << e.what(); }
		catch (LayerOutOfBounds & e) { std::cout << e.what(); }
		return nullptr;

	case CHANGE_LAYER_OPACITY:
		try {
			std::cout << "Layer depth? (0-...) "; index = getIntInput(is);
			std::cout << "Opacity? (0-100) "; opacity = getIntInput(is);
			image.getLayerObject().getLayer(index)->changeOpacity(opacity);
		}
		catch (InvalidInput & e) { std::cout << e.what(); }
		catch (LayerOutOfBounds & e) { std::cout << e.what(); }
		return nullptr;

	case SWAP_LAYERS:
		try {
			std::cout << "First layer depth? (0-...) "; index1 = getIntInput(is);
			std::cout << "Second layer depth? (0-...) "; index2 = getIntInput(is);
			image.getLayerObject().swapLayers(index1, index2);
		}
		catch (InvalidInput & e) { std::cout << e.what(); }
		catch (LayerOutOfBounds & e) { std::cout << e.what(); }
		return nullptr;

	case REMOVE_LAYER:
		try {
			std::cout << "Layer depth? (0-...) "; index = getIntInput(is);
			image.getLayerObject().removeLayer(index);
		}
		catch (InvalidInput & e) { std::cout << e.what(); }
		catch (LayerOutOfBounds & e) { std::cout << e.what(); }
		return nullptr;

	case GO_BACK:
		return new Menu();
		
	default:
		return nullptr;
	}
}
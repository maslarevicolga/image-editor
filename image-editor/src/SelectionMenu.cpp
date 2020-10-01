#include "../h/SelectionMenu.h"
#include "../h/SelectionException.h"

void SelectionMenu::print_menu() {
	std::cout
		<< "\n********************************************\n"
		<< GO_BACK << ".  Go back to the previous menu\n"
		<< CREATE_SELECTION << ".  Create selection\n"
		<< REMOVE_SELECTION << ".  Remove selection from image\n"
		<< CHANGE_SELECTION_STATUS << ".  Change selection status\n"
		<< FILL_SELECTION << ".  Fill selection\n"
		<< "Choose menu option (index of option):\n"
		<< "********************************************\n";
}

Menu* SelectionMenu::execute_option(Image& image, std::istream& is) {
	char respond;
	int x, y, w, h, r , g, b;
	bool status;
	std::string selection_name, buf;
	std::vector<Rectangle_> rec;
	switch (menu_option) {

	case CREATE_SELECTION:
		std::cout << "Selection name? ";
		std::getline(is, selection_name);
		std::cout << "Enter dimesion for selection\n";
		try {
			respond = 'y';
			while (respond == 'y') {
				std::cout << "X? ";
				x = getIntInput(is);
				if (x == -1) break;
				std::cout << "Y? ";
				y = getIntInput(is);
				std::cout << "W? ";
				w = getIntInput(is);
				std::cout << "H? ";
				h = getIntInput(is);
				rec.push_back(Rectangle_(x, y, w, h));
				std::cout << "More? (y/n) "; 
				respond = getCharInput(is);
		    }
			image.getSelectionObject().push_back(selection_name, rec);
			std::cout << "Created selection isn't active!\nIf you want to change it choose option CHANGE_SELECTION_STATUS and choose selection by name!\n ";
		}
		catch(InvalidInput& e) { std::cout << e.what();  }
		catch (SelectionExist & e) { std::cout << e.what();  }
		catch(InvalidDimension & e) { std::cout << e.what();  }
		return nullptr;

	case CHANGE_SELECTION_STATUS:
		try {
			std::cout << "Look at existing selections? (y/n) ";
			respond = getCharInput(is);
			if (respond == 'y') std::cout << image.getSelectionObject();
			std::cout << "Selection name? ";
			std::getline(is, selection_name);
			std::cout << "New status? (1/0) ";
			status = getIntInput(is);
			image.getSelectionObject().getSelection(selection_name)->changeStatus(status);
		}
		catch (InvalidInput & e) { std::cout << e.what(); }
		catch (SelectionNotFound & e) { std::cout << e.what(); }
		return nullptr;

	case REMOVE_SELECTION:
		try {
			std::cout << "Look at existing selections? (y/n) ";
			respond = getCharInput(is);
			if (respond == 'y') std::cout << image.getSelectionObject();
			std::cout << "Selection name? ";
			std::getline(is, selection_name);
			image.getSelectionObject().remove(selection_name);
		}
		catch (InvalidInput & e) { std::cout << e.what();  }
		catch (SelectionNotFound & e) { std::cout << e.what(); }
		return nullptr;

	case GO_BACK:
		return new Menu();
	case FILL_SELECTION:
		try {
			std::cout << "Look at existing selections? (y/n) ";
			respond = getCharInput(is);
			if (respond == 'y') std::cout << image.getSelectionObject();
			std::cout << "Selection name? ";
			std::getline(is, selection_name);
			std::cout << "Color in RGB format?\n";
			std::cout << "R? "; r = getIntInput(is);
			std::cout << "G? "; g = getIntInput(is);
			std::cout << "B? "; b = getIntInput(is);
			image.fill(selection_name, {r, g, b});
		}
		catch (InvalidInput & e) { std::cout << e.what(); }
		catch (SelectionNotFound& e) { std::cout << e.what(); }
		return nullptr;
		
	default:
		return nullptr;
	}
}
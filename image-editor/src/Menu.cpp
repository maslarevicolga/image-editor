#include "../h/Menu.h"
#include "../h/FormaterException.h"
#include "../h/OperationException.h"
#include "../h/LayerException.h"
#include "../h/LayerMenu.h"
#include "../h/SelectionMenu.h"
#include "../h/OperationMenu.h"
#include "../h/Xml.h"
#include "../h/XmlException.h"

#include <algorithm>

void Menu::print_menu() {
	std::cout
		<< "\n********************************************\n"
		<< LAYER << ".  Work with layers\n"
		<< SELECTION << ".  Work with selections\n"
		<< OPERATION << ".  Work with operations\n"
		<< APPLY_COMPOSITE << ".  Apply composite operation on layers\n"
		<< EXPORT_PAM_BMP << ".  Export pam or bmp or xml\n"
		<< LOAD_PROJECT << ".  Load project\n"
		<< SAVE_PROJECT << ".  Save project\n"
		<< DRAW_ON_CONSOLE << ".  Draw image on console without alpha channel\n"
		<< EXIT << ". Exit program\n"
		<< "Choose menu option (index of option):\n"
		<< "********************************************\n";
}

void Menu::read_menu_option(std::istream& is) {
	int option = getIntInput(is);
	if (option < EXIT || option > DRAW_ON_CONSOLE)
		throw InvalidMenuOption();
	else
		menu_option = static_cast<MenuOptions>(option);
}


int Menu::getIntInput(std::istream& is) const {
	std::string buff;
	std::getline(is, buff);
	if (buff.size() == 0) throw InvalidInput();
	if (std::all_of(buff.begin(), buff.end(), isdigit))
		return atoi(buff.c_str());
	throw InvalidInput();
}

char Menu::getCharInput(std::istream& is) const {
	std::string buff;
	std::getline(is, buff);
	if (buff.size() != 1) throw InvalidInput();
	return buff[0];
}

Menu* Menu::execute_option(Image& image, std::istream& is) {
	char respond;
	std::string file_path, dir_name, fun_path, file_path_dst, name;
	
	switch (menu_option) {

	case EXIT:
		try {
			if (!is_exported) {
				std::cout << "You didn't save the project!\nDo you want to exit without saving changes? (y/n) ";

				respond = getCharInput(is);
				if (respond == 'n') {
					return nullptr;
					break;
				}
			}
			is_finished = true;
		}
		catch (InvalidInput & e) { std::cout << e.what(); }
		return nullptr;

	case LAYER:
		return new LayerMenu();

	case SELECTION:
		return new SelectionMenu();

	case OPERATION:
		return new OperationMenu();

	case APPLY_COMPOSITE:
		std::cout << "Path to the FUN file? "; std::getline(is, fun_path);
	    std::cout << "Image source path? "; std::getline(is, file_path);
		std::cout << "Image destination path? "; std::getline(is, file_path_dst);

		try {
			image.readFromFile(file_path);
			image.applyOperation(fun_path);
			image.writeToFile(file_path_dst);
		}
		catch (FileNotFun & e) { std::cout << e.what(); }
		catch (OperationExistException & e) { std::cout << e.what(); }
		catch (FileNotFound & e) { std::cout << e.what(); }
		catch (InvalidOperationException & e) { std::cout << e.what(); }
		catch (InvalidFormatBMP & e) { std::cout << e.what(); }
		catch (InvalidFormatPAM & e) { std::cout << e.what(); }
		catch (OpenFileException & e) { std::cout << e.what(); }
		is_finished = true;
		return nullptr;

	case EXPORT_PAM_BMP:
		std::cout << "Out file path? ";
		std::getline(std::cin, file_path);
		try {
			image.writeToFile(file_path);
		}
		catch (OpenFileException & e) { std::cout << e.what(); }
		catch (ImageEmpty & e) { std::cout << e.what(); }
		return nullptr;

	case SAVE_PROJECT:
		std::cout << "Folder name where you want to save the project? ";
		std::getline(std::cin, dir_name);
		try {
			image.saveProject(dir_name);
		}
		catch (OpenFileException & e) { std::cout << e.what(); }
		catch (FileNotFound & e) { std::cout << e.what(); }
		catch (DirExists & e) { std::cout << e.what(); }
		catch(DirNotFound& e) { std::cout << e.what(); }
		is_exported = true;
		return nullptr;

	case LOAD_PROJECT:
		std::cout << "Project folder name? ";
		std::getline(std::cin, dir_name);
		try {
			image.loadProject(dir_name);
		}catch (FileNotFound & e) { std::cout << e.what(); }
		catch (DirNotFound & e) { std::cout << e.what(); }
		return nullptr;

	case DRAW_ON_CONSOLE:
		image.drawOnConsole();
		return nullptr;

	default:
		return nullptr;
	}
}
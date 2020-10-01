#include "../h/OperationMenu.h"
#include "../h/OperationException.h"
#include "../h/XmlException.h"
#include <cctype>
#include <iostream>
#include <iomanip>

void OperationMenu::print_menu() {
	std::cout
		<< "\n********************************************\n"
		<< GO_BACK << ".  Go to previous menu\n"
		<<  CREATE_OPERATION << ".  Create new composite operation\n"
		<< LOAD_OPERATION << ".  Load composite operation from FUN file\n"
		<< APPLY_OPERATION << ".  Execute\n"
		<< LIST_OPERATION << ".  See content of composite function\n"
		<< "Choose menu option (index of option):\n"
	    << "********************************************\n";
}


void OperationMenu::parseInput(std::string& name, int& param) const {
	int pos1 = name.find("(");
	int pos2 = name.find(")");
	if (pos1 == std::string::npos || pos2 == std::string::npos || pos1 == 0) throw InvalidInput();
	std::string n = name.substr(0, name.find("("));
	if (pos1 + 1 == pos2) 
		param = _DEFAULT_PARAM;
	else {
		std::string p = name.substr(pos1 + 1, pos2 - pos1 - 1);
		param = atoi(p.c_str());
	}
	name = n;
}

Menu* OperationMenu::execute_option(Image& image, std::istream& is) {
	int respond, index, param;
	std::string input, name;
	char rub;
	switch (menu_option) {

	case CREATE_OPERATION:
		std::cout << "Existing operation that you can combine: \n";
		std::cout << image.getOperationObject();
		std::cout << "Name of new function? ";
		std::getline(is, name);
		std::cout << "Expected input format: name(nth/param),name(nth/param)....\nDefault parametar: " << _DEFAULT_PARAM << " !\n";
		std::getline(is, input);
		try {
			image.getOperationObject().createNewComposite(name, input);
		}
		catch (OperationExistException & e) { std::cout << e.what(); }
		catch (InvalidOperationException & e) { std::cout << e.what(); }
		return nullptr;

	case LOAD_OPERATION:
		std::cout << "File path? ";
		std::getline(is, input);
		try {
			image.loadFun(input);
		}
		catch (FileNotFound & e) { std::cout << e.what(); }
		catch (FileNotFun & e) { std::cout << e.what(); }
		catch (OperationExistException & e) { std::cout << e.what(); }
		catch (InvalidOperationException & e) { std::cout << e.what(); }
		return nullptr;

	case APPLY_OPERATION:
		std::cout << image.getOperationObject();
		std::cout << "Name(nth/parameter) (default paramter: " << _DEFAULT_PARAM << " ) ";
		std::getline(is, name);
		try {
			parseInput(name, param);
			image.execute(name, param); 
		}
		catch(InvalidInput& e) { std::cout << e.what(); }
		catch (InvalidOperationException & e) { std::cout << e.what(); }
		return nullptr;

	case LIST_OPERATION:
		std::cout << image.getOperationObject();
		std::cout << "Name of composite function?\n";
		std::getline(is, name);
		try {
			if (Composite* c = dynamic_cast<Composite*>(image.getOperationObject().getOperation(name, _DEFAULT_PARAM)))
				std::cout << *c;
			else std::cout << "Funstion must be composite!\n";
		}
		catch (InvalidOperationException & e) { std::cout << e.what(); }
		return nullptr;

	case GO_BACK:
		return new Menu();

	default:
		return nullptr;
	}
}
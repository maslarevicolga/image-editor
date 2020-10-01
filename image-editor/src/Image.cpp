#include "../h/Image.h"
#include "../h/Operation.h"
#include "../h/Xml.h"
#include "../h/XmlException.h"

#include <Windows.h>
#include <algorithm>

Image* Image::instance = nullptr;

Image::Image() {}

Image* Image::createInstance() {
	if (instance != nullptr) return instance;
	instance = new Image();
	return instance;
}


void Image::readFromFile(const std::string& file_path) {
	formater = Formater::getFormater(file_path);               
	const std::vector<int> pixels = formater->read(file_path);
	layers.push_back(pixels, formater->getWidth(), formater->getHeight());
	delete formater; formater = nullptr;
}

void Image::writeToFile(const std::string& file_path) {
	const std::vector<int>& pixels = layers.merge();
	formater = Formater::getFormater(file_path);
	formater->write(file_path, pixels, layers.getWidth(), layers.getHeight());
}

void Image::fill(std::string& name, std::tuple<int, int, int> t) {
	const std::set<std::pair<int, int>> selected = selections.getSelected(name);
	if (selected.size() == 0) return;
	std::for_each(layers.begin(), layers.end(), [&selected, &t](Layer* l) { l->fillPixels(selected, t); });
}

void Image::execute(const std::string name, int param) {
	Operation* exec = operation.getOperation(name, param);
	for (auto l : layers) {
		const std::set<std::pair<int, int>>& selected = selections.getSelected();
		std::vector<int> pixelsInt = l->layerToInt();
		exec->operator()(pixelsInt, selected, l->getWidth());
		l->intToLayer(pixelsInt);
	}
}


void Image::applyOperation(const std::string& fun_path) {
	Xml* xml = new Xml();
	std::string name;
	std::string fun_vec = xml->loadFun(fun_path, name);
	operation.createNewComposite(name, fun_vec);
	execute(name);
	delete xml;
}

void Image::saveProject(const std::string& dir_name) {
	Xml* xml = new Xml();
	xml->changeDir(dir_name);
	xml->saveProject(*this);
	delete xml;
}


void Image::loadProject(const std::string& dir_name) {
	Xml* xml = nullptr;
	try {
		xml = new Xml();
		xml->changeDir(dir_name);
		throw DirNotFound();
	}catch(DirExists&) {
		xml->loadProject(*this);
	}
	delete xml;
}


void Image::loadFun(const std::string& fun_path) {
	Xml* xml = new Xml();
	std::string fun_name;
	std::string functions = xml->loadFun(fun_path, fun_name);
	operation.createNewComposite(fun_name, functions);
	delete xml;
}

void Image::drawOnConsole() const{
	std::vector<int> pix = getLayerObject().merge(); // get pixels
	HWND myconsole = GetConsoleWindow();
	HDC mydc = GetDC(myconsole);
	for (int i = 0; i < getLayerObject().getHeight(); i++) {
		for (int j = 0; j < getLayerObject().getWidth(); j++) {
			COLORREF COLOR = RGB(pix[i * getLayerObject().getWidth() + j] & 0xff,
				(pix[i * getLayerObject().getWidth() + j] >> 8) & 0xff,
				(pix[i * getLayerObject().getWidth() + j] >> 16) & 0xff);
			SetPixel(mydc, j, i, COLOR);
		}
	}
	std::cin.ignore();
	ReleaseDC(myconsole, mydc);
}
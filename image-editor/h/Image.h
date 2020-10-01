#ifndef _IMAGE_H
#define _IMAGE_H

#include "ImageSelections.h"
#include "ImageLayers.h"
#include "Image_Operation.h"
#include "Formater.h"


class Image { //singleton class

	ImageSelections selections;
	ImageLayers layers;
	Image_Operation operation;

	Formater* formater = nullptr;

	static Image* instance;
	Image();

public:

	static Image* createInstance();

	const ImageSelections& getSelectionObject() const{ return selections; }
	const ImageLayers& getLayerObject() const{ return layers; }
	const Image_Operation& getOperationObject() const{ return operation; }
	ImageSelections& getSelectionObject() { return selections; }
	ImageLayers& getLayerObject() { return layers; }
	Image_Operation& getOperationObject() { return operation; }

	void fill(std::string& name, std::tuple<int, int, int>);  //fill selection with given rgb color

	void readFromFile(const std::string&);
	void writeToFile(const std::string&);
	
	void saveProject(const std::string&);
	void loadProject(const std::string&);
	void loadFun(const std::string&);
	void execute(const std::string, int = _DEFAULT_PARAM);
	void applyOperation(const std::string&);

	void drawOnConsole() const;
};


#endif 


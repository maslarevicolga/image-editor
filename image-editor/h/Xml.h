#ifndef  XML_H
#define XML_H

#include <vector>
#include <tuple>
#include<direct.h>
#include <Windows.h>
#include <string>
#include "../xml_rapid/rapidxml.hpp"
#include <tuple>

class Rectangle_;
class Image;


class Xml {

	std::vector<std::tuple<std::string, int, int>> rel_paths_layers;
	std::vector<std::string> rel_paths_fun;
	std::vector<std::string> rel_paths_selections;
	std::string curr_dir;

	void xmlFile();
	void createHeader(rapidxml::xml_document<>*);
	void checkFun(const std::string&) const;

	std::vector<Rectangle_> loadSelection(const std::string&, bool&, std::string&); 
	
	void saveLayers(std::string s, int a, int o) { rel_paths_layers.push_back({ s, a, o }); }
	void saveSelection(const std::string&, int, std::vector<int>&);

public:
	Xml() = default;
	~Xml();
	
	void loadProject(Image&);
	void saveProject(const Image& image);

	std::string loadFun(const std::string&, std::string&);
	void saveFun(const std::string&, const std::vector<std::string>&, std::string path = "");

	void changeDir(const std::string& root_directory_name);
};




#endif

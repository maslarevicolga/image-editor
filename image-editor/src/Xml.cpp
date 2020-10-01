#include "../h/Xml.h"
#include "../h/XmlException.h"
#include "../h/Image.h"
#include "../xml_rapid/rapidxml_iterators.hpp"
#include "../xml_rapid/rapidxml_utils.hpp"
#include "../xml_rapid/rapidxml_print.hpp"
#include <iostream>
#include <regex>



void Xml::changeDir(const std::string& root_directory_name) {

    curr_dir = root_directory_name;
    int result = _mkdir(curr_dir.c_str());
    if (result == 0) return;
    if (errno == EEXIST)  // create empty dir in current directory for saving project
        throw DirExists();
    else
        if (errno == ENOENT)
            throw DirNotFound();
}


Xml::~Xml() {
    rel_paths_fun.clear();
    rel_paths_layers.clear();
    rel_paths_selections.clear();
}

void Xml::createHeader(rapidxml::xml_document<>* doc) {
    // create xml header
    rapidxml::xml_node<>* decl_node = doc->allocate_node(rapidxml::node_declaration);
    decl_node->append_attribute(doc->allocate_attribute("version", "1.0"));
    decl_node->append_attribute(doc->allocate_attribute("encoding", "utf-8"));
    doc->append_node(decl_node);
}
//<selection name="...", active="...">
//  <rectangle0>
//     <width>num</width>
//     <height>num</height>
//     <x>num</x>
//     <y>num</y>
//  </rectangle0>
//</selection>

//selection: name w h x y name w h x y
void Xml::saveSelection(const std::string& name, int active, std::vector<int>& selection) {
    rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
    createHeader(doc);
    rapidxml::xml_node<>* root = doc->allocate_node(rapidxml::node_element, "selection");
    root->append_attribute(doc->allocate_attribute("name", doc->allocate_string(name.c_str())));
    root->append_attribute(doc->allocate_attribute("active", (active == 1) ? "true" : "false"));
    int i = 0;
    auto lambda = [&doc](const std::string& c, const std::string& p) {
        rapidxml::xml_node<>* node = doc->allocate_node(rapidxml::node_element, doc->allocate_string(c.c_str()));
        node->value(doc->allocate_string(p.c_str()));
        return node;
    };
    while(i < selection.size()){;
        rapidxml::xml_node<>* rec = doc->allocate_node(rapidxml::node_element, "rectangle");
       
        rec->append_node(lambda("w", std::to_string(selection[i++])));
        rec->append_node(lambda("h", std::to_string(selection[i++]))); 
        rec->append_node(lambda("x", std::to_string(selection[i++])));
        rec->append_node(lambda("y", std::to_string(selection[i++])));
        
        root->append_node(rec);
	}
    doc->append_node(root);

    std::string xml_as_string;
    rapidxml::print(std::back_inserter(xml_as_string), *doc);
    std::ofstream fileStored(curr_dir + "\\selection_" + name + ".xml");
    rel_paths_selections.push_back(curr_dir + "\\selection_" + name + ".xml");
    fileStored << xml_as_string;
    fileStored.close();
    delete doc;

}

//<fun name="...">
//  <basic0>
//     1(10)
//  </basic0>
//</fun>

void Xml::saveFun(const std::string& name, const std::vector<std::string>& f, std::string fun_path) {
    rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
    createHeader(doc);
    rapidxml::xml_node<>* root = doc->allocate_node(rapidxml::node_element, "fun");
    root->append_attribute(doc->allocate_attribute("name", doc->allocate_string(name.c_str())));
    int i = 0;
    while (i < f.size()) {
        rapidxml::xml_node<>* b_node = doc->allocate_node(rapidxml::node_element, "basic");
        b_node->value(doc->allocate_string(f[i++].c_str()));
        root->append_node(b_node);
    }
    doc->append_node(root);

    std::string xml_as_string;
    rapidxml::print(std::back_inserter(xml_as_string), *doc);
    if (fun_path == "") {
        fun_path = curr_dir + "\\fun_" + name + ".fun";
        rel_paths_fun.push_back(curr_dir + "\\fun_" + name + ".fun");
    }
    else 
        checkFun(fun_path); 
    std::ofstream fileStored(fun_path);
    fileStored << xml_as_string;
    fileStored.close();
    delete doc;

}

//<project>
//  <layers>
//      <link>....</link>
//      ...
//  </layers>
//  <selections>
//      <link>....
//      ....
//  </selections>
//  <funs>
//      <link>....
//      ....
//  </funs> 
void Xml::saveProject(const Image& image) {
    std::string name;
    std::vector<int> dimensions;
    std::vector<std::string> funs;
    for (auto& it : image.getSelectionObject()) {
        name = it.first;
        dimensions = it.second->saveSelection();
        saveSelection(name, it.second->isActive(), dimensions);
        dimensions.clear();
    }

    for (auto& it : image.getOperationObject()) {
        funs = ((Composite*)it.second)->saveFunction();
        saveFun(it.first, funs);
        funs.clear();
    }

    int l_cnt = 0;
    std::vector<std::string> rel_paths;
    for (auto& it : image.getLayerObject()) {
        const std::vector<int>& pixels = it->layerToChar();
        Formater::getFormater(curr_dir + "\\layer" + std::to_string(l_cnt) + ".bmp")->write(curr_dir + "\\layer" + std::to_string(l_cnt) + ".bmp", pixels, it->getWidth(), it->getHeight());;
        saveLayers(curr_dir + "\\layer" + std::to_string(l_cnt++) + ".bmp", it->getIsActive(), it->getOpacity());
    }

    xmlFile();
}

void Xml::xmlFile(){
    rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
    createHeader(doc);
    rapidxml::xml_node<>* root = doc->allocate_node(rapidxml::node_element, "project");
    
    root->append_attribute(doc->allocate_attribute("about", "photoshop"));
    
    rapidxml::xml_node<>* l = doc->allocate_node(rapidxml::node_element, "layers");
    for (auto& it : rel_paths_layers) {
        rapidxml::xml_node<>* ll = doc->allocate_node(rapidxml::node_element, "layer");
        ll->append_attribute(doc->allocate_attribute("active", (std::get<1>(it) == 1) ? "true" : "false"));
        ll->append_attribute(doc->allocate_attribute("opacity", doc->allocate_string(std::to_string(std::get<2>(it)).c_str())));
        ll->value(doc->allocate_string(std::get<0>(it).c_str()));
        l->append_node(ll);
    }
    root->append_node(l);
   
    rapidxml::xml_node<>* s = doc->allocate_node(rapidxml::node_element, "selections");
    for (auto& it : rel_paths_selections) {
        rapidxml::xml_node<>* ss = doc->allocate_node(rapidxml::node_element, "selection");
        ss->value(doc->allocate_string(it.c_str()));
        s->append_node(ss);
    }
    root->append_node(s);

    rapidxml::xml_node<>* f = doc->allocate_node(rapidxml::node_element, "funs");
    for (auto& it : rel_paths_fun) {
        rapidxml::xml_node<>* ff = doc->allocate_node(rapidxml::node_element, "fun");
        ff->value(doc->allocate_string(it.c_str()));
        f->append_node(ff);

    }
    root->append_node(f);


    doc->append_node(root);

    std::string xml_as_string;
    rapidxml::print(std::back_inserter(xml_as_string), *doc);
    std::ofstream fileStored(curr_dir + "\\Project_copy.xml");
    fileStored << xml_as_string;
    fileStored.close();
    delete doc;
}


std::vector<Rectangle_> Xml::loadSelection(const std::string& sel_file, bool& status, std::string& sel_name) {
    auto lambda = [](rapidxml::xml_node<>* node, std::string s)->int {return std::atoi(node->first_node(s.c_str())->value());};
    std::vector<Rectangle_> rec;
    rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
    rapidxml::xml_node<>* root_node;

    std::ifstream file(sel_file);
    if (file.fail()) throw FileNotFound();
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc->parse<0>(&buffer[0]);
    root_node = doc->first_node("selection");
    sel_name = root_node->first_attribute("name")->value();
    status = (std::strcmp(root_node->last_attribute("active")->value(), "true") == 0) ? 1 : 0;
    for (rapidxml::xml_node<>* sel_node = root_node->first_node("rectangle"); sel_node; sel_node = sel_node->next_sibling()) {
        rec.push_back(Rectangle_(lambda(sel_node,"x"), lambda(sel_node, "y"), lambda(sel_node, "w"), lambda(sel_node, "h")));
    }
    file.close();
    delete doc;
    return rec;
}


void Xml::checkFun(const std::string& fun_path) const {
    std::regex rx("[^\\.]+\\.(.*)");
    std::smatch sm;
    std::regex_search(fun_path, sm, rx);
    if (sm.empty() == true || sm.str(1) != "fun") throw FileNotFun();
}

std::string Xml::loadFun(const std::string& fun_file, std::string& fun_name) {
    std::string res = "";
    rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
    rapidxml::xml_node<>* root_node;
    checkFun(fun_file);
    std::ifstream file(fun_file);
    if (file.fail()) throw FileNotFound();
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (buffer.size() == 0) throw FileNotFound();
    buffer.push_back('\0');

    doc->parse<0>(&buffer[0]);
    root_node = doc->first_node("fun");
    fun_name = root_node->first_attribute("name")->value();
    for (rapidxml::xml_node<>* f_node = root_node->first_node("basic"); f_node; f_node = f_node->next_sibling()) {
        res += f_node->value();
        if (f_node->next_sibling() != NULL) res += ',';
    }
    file.close();
    delete doc;
    return res;
}


void Xml::loadProject(Image& image) {
    rapidxml::xml_document<>* doc = new rapidxml::xml_document<>();
    rapidxml::xml_node<>* root_node;
    rapidxml::xml_node<>* node;
    
    std::ifstream file(curr_dir + "\\Project_copy.xml");
    if (file.fail()) throw FileNotFound();
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc->parse<0>(&buffer[0]);
    node = doc->first_node("project");
    int cnt = 0;
    for (root_node = node->first_node(); root_node; root_node = root_node->next_sibling()) {
        if (cnt == 0) {
            for (rapidxml::xml_node<>* layer_node = root_node->first_node("layer"); layer_node; layer_node = layer_node->next_sibling()) {
                image.readFromFile(layer_node->value());
                bool status = (std::strcmp(layer_node->first_attribute("active")->value(), "true") == 0) ? 1 : 0;
                int opacity = std::atoi(layer_node->last_attribute("opacity")->value());
                image.getLayerObject().getLayer(image.getLayerObject().getDepth() - 1)->changeStatus(status);
                image.getLayerObject().getLayer(image.getLayerObject().getDepth() - 1)->changeOpacity(opacity);
            }
            cnt++;
            continue;
        }
        if (cnt++ == 1) {
            for (rapidxml::xml_node<>* selection_node = root_node->first_node("selection"); selection_node; selection_node = selection_node->next_sibling()) {
                std::string selection_name;
                bool status;
                std::vector<Rectangle_> rec = loadSelection(selection_node->value(), status, selection_name);
                image.getSelectionObject().push_back(selection_name, rec);
                image.getSelectionObject().getSelection(selection_name)->changeStatus(status);
            }
            continue;
        }
        for (rapidxml::xml_node<>* fun_node = root_node->first_node("fun"); fun_node; fun_node = fun_node->next_sibling()) {
            std::string fun_name;
            std::string fun_vec = loadFun(fun_node->value(), fun_name);
            image.getOperationObject().createNewComposite(fun_name, fun_vec);
        }
    }
   
    file.close();
    delete doc;
}
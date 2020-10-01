#include "../h/Image_Operation.h"
#include "../h/OperationException.h"
#include <regex>

void Image_Operation::initialize() {
	operations.push_back({ "add", new Add() });
	operations.push_back({ "sub", new Sub() });
	operations.push_back({ "sub_inverse", new Sub_Inv() });
	operations.push_back({ "mul", new Mul() });
	operations.push_back({ "div", new Div() });
	operations.push_back({ "div_inverse", new Div_Inv() });
	operations.push_back({ "pow", new Pow() });
	operations.push_back({ "log", new Log() });
	operations.push_back({ "abs", new Abs() });
	operations.push_back({ "max", new Max() });
	operations.push_back({ "min", new Min() });
	operations.push_back({ "inversion", new Inversion() });
	operations.push_back({ "grayscale", new Grayscale() });
	operations.push_back({ "black_white", new Black_White() });
	operations.push_back({ "median", new Median() });
}

std::vector<std::pair<const std::string, Operation*>>::const_iterator Image_Operation::getStart()const {
	if (operations.size() == _BASIC_OPERATIONS)
		return operations.end();
	int cnt = 0;
	auto it = operations.begin();
	while (cnt++ < _BASIC_OPERATIONS)
		it++;
	return it;
}


Operation* Image_Operation::check_valid(const std::string& name) const {
	std::vector<std::pair<const std::string, Operation*>>::const_iterator it = operations.begin();
	while (it != operations.end() && it->first != name) 
		it++;
	return (it != operations.end()) ? it->second : nullptr;
}

Operation* Image_Operation::getOperation(const std::string& name, int param = _DEFAULT_PARAM) {
	Operation* op = check_valid(name);
	if(!op) throw InvalidOperationException();
	op->setParam(param);
	return op;
}

// 1(3),2(8),f() 
void Image_Operation::createNewComposite(const std::string& name, std::string& input) {
	Operation* ch = check_valid(name);
	if (ch != nullptr) throw OperationExistException();
	input += ",";
	std::vector<Operation*> tmp;
	std::regex rx("([^,]*),");
	std::sregex_iterator begin(input.begin(), input.end(), rx);
	std::sregex_iterator end;
	while (begin != end) {
		std::smatch result = *begin;
		if (result.empty() == true) throw InvalidOperationException();
		std::string one = result.str(1);
		std::regex rx2("([^\\(]+)\\(([^\\)]*).*");
		std::smatch result2;
		std::regex_match(one, result2, rx2);
		if (result2.empty() == true) throw InvalidOperationException();
		std::string part_name = result2.str(1);
		int param = _DEFAULT_PARAM;
		Operation* op = check_valid(part_name);
		if (!op) throw InvalidOperationException();
		if (result2.str(2).empty() == false)
			param = std::atoi(result2.str(2).c_str());
		tmp.push_back(op->copy(param));
		
		begin++;
	}
	operations.push_back({ name, new Composite(tmp, name) });
}

Image_Operation::~Image_Operation() {
	for (auto& it : operations) {
		std::string name = it.second->getName();
		int pos1 = name.find("(");
		int pos2 = name.find(")");
		name = name.substr(0, name.find("("));
		if (pos1 + 1 == pos2) removeFromComposite(name);
		delete it.second;
		it.second  = nullptr;
	}
	operations.clear();
}

void Image_Operation::removeFromComposite(const std::string& name) {
	for (auto it = getStart(); it != operations.end(); it++)
		if (it->second == nullptr) continue;
		else
			((Composite*)(it->second))->remove(name);
}
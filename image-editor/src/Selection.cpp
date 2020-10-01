#include "../h/Selection.h"


Selection::~Selection() { 
	rectangles.clear();
	std::vector<Rectangle_>().swap(rectangles);
}

void Selection::changeStatus(bool status) {
	is_active = status;
}


std::vector<int> Selection::saveSelection() const {
	std::vector<int> dimensions;
	for (auto& rec : rectangles) {
		dimensions.push_back(rec.getW());
		dimensions.push_back(rec.getH());
		dimensions.push_back(rec.getX());
		dimensions.push_back(rec.getY());
	}
	return dimensions;
}
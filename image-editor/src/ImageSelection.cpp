#include "../h/ImageSelections.h"
#include "../h/SelectionException.h"

#include <algorithm>

void ImageSelections::push_back(const std::string name, std::vector<Rectangle_> rec) {
	if (all_selections.find(name) == all_selections.end())
		all_selections.insert({ name, new Selection(name, rec) });
	else throw SelectionExist();
}

std::set<std::pair<int, int>> ImageSelections::getSelected(const std::string& name) const {
	std::set<std::pair<int, int>> selected;
	Selection* sel = getSelection(name);
	if (sel->isActive() == false) return selected;
	for (const Rectangle_& rec : *sel)
		for (int i = rec.getY(); i < rec.getY() + rec.getH(); i++)
			for (int j = rec.getX(); j < rec.getX() + rec.getW(); j++)
				selected.insert(std::make_pair(j, i));
	return selected;
}

std::set<std::pair<int, int>> ImageSelections::getSelected() const {
	std::set<std::pair<int, int>> selected;
	for (auto& it : *this) {
		if (it.second->isActive() == false) continue;
		for (const Rectangle_& rec : *it.second)
			for (int i = rec.getY(); i < rec.getY() + rec.getH(); i++)
				for (int j = rec.getX(); j < rec.getX() + rec.getW(); j++)
					selected.insert(std::make_pair(j, i));
	}
	return selected;
}

Selection* ImageSelections::getSelection(const std::string& name) const {
	auto it = all_selections.find(name);
	if (it == all_selections.end()) throw SelectionNotFound();
	return it->second;
}

void ImageSelections::changeStatus(const std::string& name, bool status) {
	getSelection(name)->changeStatus(status);
}

void ImageSelections::remove(const std::string& name) {
	Selection* found = getSelection(name);
	all_selections.erase(name);
	delete found; found = nullptr;
}



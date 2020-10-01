#ifndef _IMAGE_SELECTIONS_H
#define _IMAGE_SELECTIONS_H

#include <map>
#include <string>
#include <set>
#include "Selection.h"

class ImageSelections {

	std::map<const std::string, Selection*> all_selections;

public:

	std::map<const std::string, Selection*>::const_iterator begin() const { return all_selections.begin(); }
	std::map<const std::string, Selection*>::const_iterator end() const { return all_selections.end(); }

	void push_back(const std::string, std::vector<Rectangle_>);
	Selection* getSelection(const std::string&) const;
	std::set<std::pair<int, int>> getSelected(const std::string&) const;
	std::set<std::pair<int, int>> getSelected() const;
	void changeStatus(const std::string&, bool);
	void remove(const std::string&);


	friend std::ostream& operator<<(std::ostream& os, const ImageSelections& s) {
		for (auto it : s.all_selections)
			os << *it.second << "\n";
		return os;
	}
};



#endif
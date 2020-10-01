#ifndef _SELECTION_H
#define _SELECTION_H

#include "Rectangle.h"
#include <vector>
#include <iostream>
#include <set>

class Selection {

	std::string selection_name;
	std::vector<Rectangle_> rectangles;
	bool is_active;
	
public:
	Selection(const std::string& name, std::vector<Rectangle_>& rec, bool status = false) : selection_name(name), rectangles(rec), is_active(status) {}

	~Selection();

	std::vector<Rectangle_>::iterator begin() { return rectangles.begin(); }
	std::vector<Rectangle_>::iterator end() { return rectangles.end(); }
	std::vector<Rectangle_>::const_iterator begin() const { return rectangles.begin(); }
	std::vector<Rectangle_>::const_iterator end() const { return rectangles.end(); }

	void changeStatus(bool status);

	bool isActive() const { return is_active; }
	const std::string& getName() const { return selection_name; }

	std::vector<int> saveSelection() const;

	friend std::ostream& operator<<(std::ostream& os, const Selection& s) {
		os << s.selection_name << " " << s.isActive() << "\n";
		for (auto it : s)
			os << it << "\n";
		return os;
	}
};





#endif
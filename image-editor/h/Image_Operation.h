#ifndef _IMAGE_OPERATION_H
#define _IMAGE_OPERATION_H

#include "Operation.h"
#include <string>
#include <vector>
#include <iostream>

#define _BASIC_OPERATIONS 15
#define _DEFAULT_PARAM 50

class Image_Operation {

	std::vector<std::pair<const std::string, Operation*>> operations;

	void initialize();
	Operation* check_valid(const std::string&) const;
	std::vector<std::pair<const std::string, Operation*>>::const_iterator getStart()const;

	void removeFromComposite(const std::string&);
public:
	
	Image_Operation() { initialize(); }

	std::vector<std::pair<const std::string, Operation*>>::const_iterator begin() const { return getStart(); }
	std::vector<std::pair<const std::string, Operation*>>::const_iterator begin() { return getStart(); }
	std::vector<std::pair<const std::string, Operation*>>::const_iterator end() const { return operations.end(); }

	Operation* getOperation(const std::string&, int);
	void createNewComposite(const std::string&, std::string&);

	friend std::ostream& operator<<(std::ostream& os, const Image_Operation& io) {
		int cnt = 1;
		for (auto& it : io.operations)
			os << cnt++ << ". " << it.second->getName() << "\n";
		return os;
	}

	~Image_Operation();

};






#endif
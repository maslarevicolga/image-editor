#ifndef FORMATER_H
#define FORMATER_H

#include <vector>
#include <string>

class Formater {

protected:
	int width, height;
public:
	static Formater* getFormater(const std::string&);
	virtual std::vector<int> read(const std::string&) = 0;
	virtual void write(const std::string&, const std::vector<int>&, int, int) = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
};



#endif

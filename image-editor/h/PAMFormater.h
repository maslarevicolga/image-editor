#ifndef _PAM_FORMATER_H
#define _PAM_FORMATER_H

#include "Formater.h"

class PAM_Formater : public Formater {
public:
	std::vector<int> read(const std::string&) override;
	void write(const std::string&, const std::vector<int>&, int, int) override;
	int getWidth() const override { return width; }
	int getHeight() const override { return height; }
};





#endif
#ifndef _BMP_FORMATER_H
#define _BMP_FORMATER_H

#include "Formater.h"


#define BMP_FILE_HEADER 14
#define BMP_INFO_HEADER 40
#define BMP_COLOR_HEADER 84


class BMP_Formater : public Formater {

	bool verify_BMP(unsigned char* buff) {
		return (buff[0] == 'B' && buff[1] == 'M');
	}
	unsigned int add_padding(unsigned int row_size);
	unsigned char* get_file_header(int w, int h);
	unsigned char* get_info_header(int w, int h);

public:

	std::vector<int> read(const std::string&) override;
	void write(const std::string&, const std::vector<int>&, int, int) override;
	int getWidth() const override { return width; }
	int getHeight() const override { return height; }
};

#endif
#include "../h/BMPFormater.h"
#include "../h/FormaterException.h"
#include <iostream>
#include<fstream>

unsigned int BMP_Formater::add_padding(unsigned int row_size) {
	unsigned new_row_size = row_size;
	while (new_row_size % 4 != 0)
		new_row_size++;
	return new_row_size;
}

std::vector<int> BMP_Formater::read(const std::string& src_path) {
	std::ifstream rf(src_path, std::ios::out | std::ios::binary);
	if (!rf) throw OpenFileException();

	unsigned char* buff = new unsigned char[BMP_FILE_HEADER];
	rf.read((char*)buff, BMP_FILE_HEADER);
	if (verify_BMP(buff) == false)
		throw InvalidFormatBMP();
	unsigned int offset_data = *(unsigned int*)(buff + 10);
	delete[] buff;

	buff = new unsigned char[BMP_INFO_HEADER];
	rf.read((char*)buff, BMP_INFO_HEADER);
	width = *(int*)(buff + 4);
	height = *(int*)(buff + 8);
	unsigned short bits_per_pixel = *(unsigned short*)(buff + 14);
	delete[] buff; buff = nullptr;

	rf.seekg(offset_data, rf.beg);
	if (height < 0 || (bits_per_pixel != 32 && bits_per_pixel != 24))
		throw InvalidFormatBMP();
	unsigned int row_size = width * 4;
	if (bits_per_pixel / 8 != 4) row_size = add_padding(width * 3);

	std::vector<int> pixels; pixels.assign(width * height, 0);
	buff = new unsigned char[row_size];
	for (int y = 0; y < height; y++) {
		rf.read((char*)buff, row_size);
		for (int x = 0; x < width; x++)
			if (bits_per_pixel / 8 == 3) pixels[x + (height - 1 - y) * width] = (buff[3 * x + 2] | (buff[3 * x + 1] << 8) | (buff[3 * x] << 16) | (0xff << 24));
			else 
				pixels[x + (height - 1 - y) * width] = (buff[4 * x + 2] | (buff[4 * x + 1] << 8) | (buff[4 * x] << 16) | (buff[4 * x + 3] << 24));
	}
	delete[] buff; buff = nullptr;
	rf.close();
	return pixels;
}

unsigned char* BMP_Formater::get_file_header(int w, int h) {
	unsigned char* file_header = new unsigned char[BMP_FILE_HEADER];

	auto fun = [file_header](int index, uint8_t value) {file_header[index] = value; };

	fun(0, 'B'); fun(1, 'M');
	unsigned int file_size = 14 + 124 + w * h * 4;
	fun(2, file_size & 0x000000ff); fun(3, (file_size >> 8) & 0x000000ff); fun(4, (file_size >> 16) & 0x000000ff); fun(5, (file_size >> 24) & 0x000000ff); fun(6, 0x00); fun(7, 0x00); fun(8, 0x00); fun(9, 0x00);
	unsigned int offset = 14 + 124;
	fun(10, offset & 0x000000ff); fun(11, (offset >> 8) & 0x000000ff); fun(12, (offset >> 16) & 0x000000ff); fun(13, (offset >> 24) & 0x000000ff);

	return file_header;
}

unsigned char* BMP_Formater::get_info_header(int w, int h) {
	unsigned char* file_header = new unsigned char[BMP_INFO_HEADER + BMP_COLOR_HEADER]{
		0x7C, 0x00, 0x00, 0x00,
		(uint8_t)(w & 0x000000ff), (uint8_t)((w >> 8) & 0x000000ff), (uint8_t)((w >> 16) & 0x000000ff), (uint8_t)((w >> 24) & 0x000000ff),
		(uint8_t)(h & 0x000000ff), (uint8_t)((h >> 8) & 0x000000ff), (uint8_t)((h >> 16) & 0x000000ff), (uint8_t)((h >> 24) & 0x000000ff),
		0x01, 0x00, 0x20, 0x00,
		0x03, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x2C, 0x01, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFF, 0x00,
		0x00, 0xFF, 0x00, 0x00,
		0xFF, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xFF,
		0x73, 0x52, 0x47, 0x42,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	return file_header;
}

void BMP_Formater::write(const std::string& dst_path, const std::vector<int>& pixels, int w, int h) {
	std::ofstream wf(dst_path, std::ios::out | std::ios::binary);
	if (!wf) throw OpenFileException();
	unsigned char* file_header = get_file_header(w, h);
	wf.write((const char*)file_header, BMP_FILE_HEADER);
	delete[] file_header;

	file_header = get_info_header(w, h);
	wf.write((const char*)file_header, BMP_INFO_HEADER + BMP_COLOR_HEADER);
	delete[] file_header; file_header = nullptr;
	
	for (int y = 0; y < h; y++) 
		for (int x = 0; x < w; x++) {
			char buff[4]; 
			buff[0] = (pixels[x + (h - 1 - y) * w] >> 16) & 0xff;
			buff[1] = (pixels[x + (h - 1 - y) * w] >> 8) & 0xff;
			buff[2] = (pixels[x + (h - 1 - y) * w]) & 0xff;
			buff[3] = (pixels[x + (h - 1 - y) * w] >> 24) & 0xff;
			wf.write((const char*)buff, 4);
	}
	wf.close();
}

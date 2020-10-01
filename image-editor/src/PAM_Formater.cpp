#include "../h/PAMFormater.h"
#include "../h/FormaterException.h"
#include <fstream>


std::vector<int> PAM_Formater::read(const std::string& src_path) {
	std::ifstream ifs;
	ifs.open(src_path, std::ios::binary);
	if (ifs.fail()) throw OpenFileException();

	std::string header;
	ifs >> header;
	if (strcmp(header.c_str(), "P7") != 0) throw InvalidFormatPAM();

	int depth;
	ifs >> header >> width;
	ifs >> header >> height;
	ifs >> header >> depth;

	int max_val;
	std::string tupltype;
	ifs >> header >> max_val;
	ifs >> header >> tupltype;
	if (tupltype != "RGB" && tupltype != "RGB_ALPHA") throw InvalidFormatPAM();
	ifs >> header;
	
	unsigned int pix_size = 3;
	if (tupltype == "RGB_ALPHA") pix_size = 4;
	
	unsigned char* tmp = new unsigned char;
	std::vector<int> pixels; pixels.assign(width * height, 0);
	unsigned char* pix_buffer = new unsigned char[pix_size];
	
	while (1) {
		ifs.read((char*)tmp, 1);
		if (*tmp != '\n') break;
	}
	for(int i = 0; i < width * height; i++){
		if (i == 0) {
			pix_buffer[0] = *tmp;
			for (int j = 1; j < pix_size; j++) { ifs.read((char*)tmp, 1);  pix_buffer[j] = *tmp; }
		}
		else ifs.read((char*)pix_buffer, pix_size);
		if (pix_size == 3) pixels[i] = (pix_buffer[0] | pix_buffer[1] << 8 | pix_buffer[2] << 16 | 0xff << 24);
		else
		if (pix_size == 4) pixels[i] = *(int*)pix_buffer;
	}
	delete[] pix_buffer; pix_buffer = nullptr;
	ifs.close();
	return pixels;
}


void PAM_Formater::write(const std::string& dst_path, const std::vector<int>& pixels, int w, int h) {
	std::ofstream ofs;
	ofs.open(dst_path, std::ios::binary);
	if (ofs.fail()) throw OpenFileException();

	ofs << "P7\n";
	ofs << "WIDTH " << w << '\n';
	ofs << "HEIGHT " << h << '\n';
	ofs << "DEPTH " << 4 << '\n';
	ofs << "MAXVAL " << 255 << '\n';
	ofs << "TUPLTYPE RGB_ALPHA\n";
	ofs << "ENDHDR\n";
	unsigned char pix_buffer[4];
	
	for (int i = 0; i < w * h; i++) {
		pix_buffer[0] = pixels[i] & 0xff;
		pix_buffer[1] = (pixels[i] >> 8)& 0xff;
		pix_buffer[2] = (pixels[i] >> 16) & 0xff;
		pix_buffer[3] = (pixels[i] >> 24) & 0xff;
		ofs.write((char*)pix_buffer, 4);
	}
	ofs.close();
}
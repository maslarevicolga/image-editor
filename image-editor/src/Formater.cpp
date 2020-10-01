#include "../h/Formater.h"
#include "../h/BMPFormater.h"
#include "../h/PAMFormater.h"
#include "../h/FormaterException.h"
#include <regex>


Formater* Formater::getFormater(const std::string& src_path) {
	std::regex rx("[^\\.]+\\.(.*)");
	std::smatch sm;
	std::regex_search(src_path, sm, rx);
	if (sm.empty() == true) throw OpenFileException();
	if (sm.str(1) == "bmp") return new BMP_Formater();
	if (sm.str(1) == "pam") return new PAM_Formater();
	else throw OpenFileException();
}




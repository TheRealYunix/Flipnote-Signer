#include "PPM.hpp"
#include <windows.h>

char PPM::calc_check_digit(const std::string& filename) {
	std::string checksumDict = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int sumc;
	try {
		sumc = std::stoi(filename.substr(0, 2), nullptr, 16);
	}
	catch (const std::invalid_argument& ia) {
		return -1;
	}

	for (int i = 1; i < 16; ++i) {
		int charVal = static_cast<int>(filename[i]);
		sumc = (sumc + charVal) % 256;
	}

	return checksumDict[sumc % checksumDict.length()];
}
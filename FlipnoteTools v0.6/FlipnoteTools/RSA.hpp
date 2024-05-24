#pragma once
#include <iostream>
#include <sstream>

namespace RSA {
		std::string SignString(std::string &message);
		bool Sign(const char* filename);
		bool CheckSign(const char* filename);
};

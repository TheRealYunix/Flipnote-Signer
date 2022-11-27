#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

//	File exteension by Yunix for C++
//	UTF-8 (can be used as UTF-16 with std::wstring)

std::string string_to_hex(const std::string& input);
namespace File
{
	//Normal
	bool Write(const char *filename, const std::string buffer, int Seek, int SeekFlag);
	std::string Read(const char *filename, uint32_t size, int seek, int SeekFlag);

	//Binary
	bool WriteBinary(const char *filename, std::string buffer, int Seek, int SeekFlag);
	std::string ReadBinary(const char *filename, uint32_t size, int seek, int SeekFlag);
};
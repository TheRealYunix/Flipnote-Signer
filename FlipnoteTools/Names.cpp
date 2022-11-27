#include "Flipnote.hpp"



std::string utf8_encode(const std::wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str, int codePage = GetACP())
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

namespace PPM
{


	void SetCreatorName(const char *filename, std::string u8)
	{
		std::ofstream fs;
		fs.open(filename, std::ios::out | std::ios::in);
		fs.seekp(0x40, std::ios::beg);
		fs.write((char *)utf8_decode(u8).c_str(), 0x14);
		fs.close();
	}


	void SetOriginalCreatorName(const char *filename, std::string u8)
	{
		std::ofstream fs;

		fs.open(filename, std::ios::out | std::ios::in);
		fs.seekp(0x14, std::ios::beg);
		fs.write((char *)utf8_decode(u8).c_str(), 0x14);
		fs.seekp(0x2A, std::ios::beg);
		fs.write((char *)utf8_decode(u8).c_str(), 0x14);
		fs.close();
	}

	std::string OriginalCreatorName(const char *filename)
	{
		std::ifstream fin(filename, std::ios::binary);
		if (fin.tellg() > 0x1E0)
			return 0;
		fin.seekg(0, std::ios::end);
		size_t size = (size_t)fin.tellg();

		//skip BOM
		fin.seekg(0x14, std::ios::beg);
		size -= 2;

		std::wstring u16((size / 2) + 1, '\0');
		fin.read((char*)&u16[0], 0x14);

		return utf8_encode(u16);
	}

	std::string CreatorName(const char *filename)
	{
		std::ifstream fin(filename, std::ios::binary);
		if (!ValidPPM(filename))
			return 0;
		fin.seekg(0, std::ios::end);
		size_t size = (size_t)fin.tellg();

		//skip BOM
		fin.seekg(0x40, std::ios::beg);
		size -= 2;

		std::wstring u16((size / 2) + 1, '\0');
		fin.read((char*)&u16[0], 0x14);

		return utf8_encode(u16);
	}

	bool isLocked(const char* filename)
	{
		return  std::stoi(File::ReadBinary(filename, 0x1, 0x10, std::ios::beg).c_str());
	}
};
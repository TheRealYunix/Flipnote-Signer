#include "Flipnote.hpp"
#include <windows.h>
#include "File.hpp"
#include <codecvt>
#include <vector>
#include <bitset>
#include <limits>

std::string ReverseHex(std::string input)
{
	std::string output = input;
	std::reverse(output.begin(), output.end());
	for (size_t i = 1; i < output.size(); i += 2)
		std::swap(output[i - 1], output[i]);
	return output;
}

namespace PPM
{


	void SetOriginalAuthorID(const char *filename, std::string u8)
	{
		std::cout << "Original AuthorID: " << ReverseHex(u8) << std::endl;
		File::WriteBinary(filename, ReverseHex(u8), 0x8A, std::ios::beg);
		File::WriteBinary(filename, ReverseHex(u8), 0x56, std::ios::beg);
	}

	void SetAuthorID(const char *filename, std::string u8)
	{
		std::cout << "AuthorID: " << ReverseHex(u8) << std::endl;
		File::WriteBinary(filename, ReverseHex(u8), 0x5E, std::ios::beg);
	}

	std::string AuthorID(const char *filename)
	{
		return ReverseHex(File::ReadBinary(filename, 0x8, 0x5E, std::ios::beg));
	}
	std::string OriginalAuthorID(const char *filename)
	{
		return ReverseHex(File::ReadBinary(filename, 0x8, 0x56, std::ios::beg));
	}

	bool ValidPPM(const char *filename)
	{
		std::ifstream PPMFILE(filename, std::ios::ate | std::ios::binary);
		if (PPMFILE.tellg() < 0x1E0)
		{
			PPMFILE.close();
			return false;
		}
		else
		{
			PPMFILE.close();
			return true;
		}
	}




};

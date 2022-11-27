#include "PPM_Filename.hpp"


bool FirstLetterHEx(std::string s)
{
	return s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}

bool isNumber(const std::string& s)
{
	for (char const &ch : s) {
		if (isdigit(ch) == 0)
			return false;
	}
	return true;
}
namespace PPM
{
	namespace Name
	{

		
		void Write_Hexname(const char *filename, std::string newname)
		{
			std::cout << "----------FILENAME---------\n\n\n";
			std::cout << newname << std::endl;
			if (FirstLetterHEx(newname))
			{
				File::WriteBinary(filename, newname, 0x78, std::ios::beg);
			}
			else
			{
				File::WriteBinary(filename, newname, 0x78, std::ios::beg);
			}
		//	File::WriteBinary(filename, newname, 0x78, std::ios::beg);
		//	File::WriteBinary(filename, newname, 0x92, std::ios::beg);
		}
		void Write_centralname(const char *filename, std::string newname)
		{
			File::Write(filename, newname, 0x7B, std::ios::beg);
		}
		void Write_endname(const char *filename, std::string newname)
		{
			if (isNumber)
			{
				int x = std::stoi(newname);
				std::stringstream ss;
				ss << std::hex << x;
				std::cout << ss.str() << std::endl;
				std::string result(ss.str());
				if (result.length() > 2)
				{
					std::string half = result.substr(0, result.length() / 2);
					std::string otherHalf = result.substr(result.length() / 2);
					File::WriteBinary(filename, otherHalf, 0x88, std::ios::beg);
					File::WriteBinary(filename, half, 0x89, std::ios::beg);
				}
				else
				{
					File::WriteBinary(filename, result, 0x88, std::ios::beg);
					File::WriteBinary(filename, "00", 0x89, std::ios::beg);

				}
			}

			/*int x = std::stoi(newname);
			std::stringstream ss;
			ss << std::hex << x;
			std::cout << ss.str() <<std::endl;
			std::string result(ss.str());
			if (result.length() < 3)
			{
				
				File::WriteBinary(filename, result, 0x88, std::ios::beg);
			}
			else
			{
				result += "0";
				std::reverse(result.begin(), result.end());
				File::WriteBinary(filename, result, 0x88, std::ios::beg);
			}*/
		}


		std::string Hexname(const char *filename)
		{
			return File::ReadBinary(filename, 0x3, 0x78, std::ios::beg);
		}
		std::string centralname(const char *filename)
		{
			return	File::Read(filename, 13, 0x7B, std::ios::beg).erase(13, -1);

		}
		std::string endname(const char *filename)
		{
			

			std::string result = File::ReadBinary(filename, 0x1, 0x89, std::ios::beg) + File::ReadBinary(filename, 0x1, 0x88, std::ios::beg);

			unsigned int i;
			std::istringstream iss(result);
			iss >> std::hex >> i;

			if (i>99)
				return std::to_string(i);
			if(i < 10)
				return "00" + std::to_string(i);
			else
				return "0" + std::to_string(i);
			/*
			std::stringstream stream;
			std::string endname = File::ReadBinary(filename, 0x2, 0x88, std::ios::beg);
			
			std::reverse(endname.begin(), endname.end());
			endname.pop_back();

			unsigned int x;
			std::stringstream ss;
			ss << std::hex << endname;
			ss >> x;

			return std::to_string(x);*/
		}
		std::string Filename(const char *filename)
		{
			return Hexname(filename) + "_" + centralname(filename) + "_" + endname(filename);
		}
	};
};
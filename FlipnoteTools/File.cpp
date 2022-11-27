#include "File.hpp"

namespace File
{
	
	std::string Read(const char *filename, uint32_t size, int seek, int SeekFlag)
	{
		std::vector<char> buffer(size);
		std::ifstream File(filename);
		File.seekg(seek, SeekFlag);
		File.read(buffer.data(), size);
		File.close();

			File.close();
			return buffer.data();
		
	
	}

	bool Write(const char *filename, const std::string buffer, int Seek, int SeekFlag)
	{
		std::ofstream File(filename, std::ios::in);
		File.seekp(Seek, SeekFlag);
		if (!File.write((char *)buffer.c_str(), buffer.size()))
			return false;
		else
		{
			File.close();
			return true;
		}
	}

};
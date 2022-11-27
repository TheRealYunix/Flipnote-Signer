#include "Flipnote.hpp"

namespace PPM
{
	bool Sign(const char* filename)
	{

		std::ifstream input(filename, std::ios::binary);
		std::string str(std::istreambuf_iterator<char>(input), {});
		if (!ValidPPM(filename))
		{
			MessageBox(nullptr, "file is corrupt, this is not a valid PPM file!", "PPM Error", MB_ICONERROR);
			return false;
		}
		else
		{
			str.erase(str.size() - 0x90);
			std::string Hash = RSA::SignString(str);
			input.close();

			std::ofstream File(filename, std::ios::out |std::ios::in | std::ios::binary);

			std::basic_string<uint8_t> bytes;
			//For every Byte we are convert the string letter to Hex
			for (size_t i = 0; i < Hash.length(); i += 2)
			{
				uint16_t byte;
				std::string nextbyte = Hash.substr(i, 2);
				std::istringstream(nextbyte) >> std::hex >> byte;
				bytes.push_back(static_cast<uint8_t>(byte));
			}
			std::string result(begin(bytes), end(bytes));
			File.seekp(-0x90, std::ios::end);

			File.write((char *)result.c_str(), result.length());
				//		return false;
			return true;
			std::cout << "signed succesfully!\n";
		}
	}


	bool CheckSign(const char* filename)
	{

		switch (ValidPPM(filename))
		{
		case false:
			MessageBox(nullptr, "file is corrupt, this is not a valid PPM file!", "PPM Error", MB_ICONERROR);
			return false;
			break;

		case true:

			std::ifstream input(filename, std::ios::binary);
			std::string str(std::istreambuf_iterator<char>(input), {});
			str.erase(str.size() - 0x90);
			std::string OriginalHash;
			OriginalHash = RSA::SignString(str);
			input.close();

			if (File::ReadBinary(filename, 0x80, -0x90, std::ios::end) != OriginalHash)
				return false;
			else
				return true;
			break;
		}
	}
};
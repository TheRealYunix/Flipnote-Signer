#include "File.hpp"


std::string string_to_hex(const std::string& input)
{
	static const char hex_digits[] = "0123456789ABCDEF";

	std::string output;
	output.reserve(input.length() * 2);
	for (unsigned char c : input)
	{
		output.push_back(hex_digits[c >> 4]);
		output.push_back(hex_digits[c & 15]);
	}
	return output;
}

namespace File
{
	

	bool WriteBinary(const char *filename, std::string buffer, int Seek, int SeekFlag)
	{
		std::ofstream File(filename, std::ios::out | std::ios::in | std::ios::binary);

		std::basic_string<uint8_t> bytes;
		//For every Byte we are convert the string letter to Hex
		for (size_t i = 0; i < buffer.length(); i += 2)
		{
			uint16_t byte;
			std::string nextbyte = buffer.substr(i, 2);
			std::istringstream(nextbyte) >> std::hex >> byte;
			bytes.push_back(static_cast<uint8_t>(byte));
		}
		std::string result(begin(bytes), end(bytes));
		File.seekp(Seek, SeekFlag);

		File.write((char *)result.c_str(), result.length());
	}
	
	std::string ReadBinary(const char *filename, uint32_t size, int seek, int SeekFlag)
	{
		std::ifstream File(filename, std::ios::in | std::ios::binary);

		std::vector<char> buffer(size);

		File.seekg(seek, SeekFlag);
		File.read(buffer.data(), size);

		std::string Buf;

		for (int i = 0; i < size; i++)
		{
			Buf += buffer[i];
		}
		File.close();
		return string_to_hex(Buf);
	}
};
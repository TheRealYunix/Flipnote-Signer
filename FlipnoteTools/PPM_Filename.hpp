#include "File.hpp"

namespace PPM
{
	namespace Name
	{

		void Write_Hexname(const char *filename, std::string newname);
		void Write_centralname(const char *filename, std::string newname);
		void Write_endname(const char *filename, std::string newname);

		std::string Filename(const char *filename);
	
		std::string Hexname(const char *filename);
		std::string centralname(const char *filename);
		std::string endname(const char *filename);
	};
};
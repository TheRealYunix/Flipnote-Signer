#include <iostream>
#include <string>
#include "Signer.hpp"
#include <fstream>
#include <sstream>
#include <windows.h>
#include "PPM_Filename.hpp"
#include "File.hpp"

namespace PPM
{
	std::string Filename(const char *filename);

	//ID
	std::string AuthorID(const char *filename);
	std::string OriginalAuthorID(const char *filename);
	void SetAuthorID(const char *filename, std::string u8);
	void SetOriginalAuthorID(const char *filename, std::string u8);


	std::string CreatorName(const char *filename);
	std::string OriginalCreatorName(const char *filename);
	void SetCreatorName(const char *filename, std::string u8);
	void SetOriginalCreatorName(const char *filename, std::string u8);


	bool ValidPPM(const char *filename);
	bool isLocked(const char* filename);
	bool Sign(const char* filename);
	bool CheckSign(const char* filename);
};
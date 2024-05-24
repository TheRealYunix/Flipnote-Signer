#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <codecvt>
#include <vector>
#include <cctype>


namespace convert
{
	bool IsHexadecimal(const std::string& str)
	{
		if (!isxdigit(str[0]))
		{
			return false;
		}

		for (size_t i = 1; i < str.size(); ++i)
		{
			if (!isxdigit(str[i]))
			{
				return false;
			}
		}

		return true;
	}
	std::string wstring_to_string(const std::wstring& wstr) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}

	std::vector<uint8_t> wstring_to_uint8_t(const std::wstring& wstr) {
		std::vector<uint8_t> result;
		for (wchar_t wc : wstr) {
			result.push_back(static_cast<uint8_t>(wc));
		}
		return result;
	}

	uint32_t reversedHexToUint32(uint32_t reversedHex) {
		return ((reversedHex & 0xFF) << 24) |
			((reversedHex & 0xFF00) << 8) |
			((reversedHex & 0xFF0000) >> 8) |
			((reversedHex & 0xFF000000) >> 24);
	}

	std::wstring author_ids_to_wstring(uint64_t authorId)
	{
		wchar_t Str[17];
		swprintf_s(Str, L"%016llX", authorId);
		return Str;
	}

	uint64_t string_to_author_ids(std::string authorId)
	{
		uint64_t authorIDValue;
		sscanf_s(authorId.c_str(), "%016llX", &authorIDValue);
		return authorIDValue;
	}


	std::string format_filename(const char* filename)
	{
		std::string str(filename);

		// Extract MAC address
		std::string mac = str.substr(0, 6);

		// Extract 13-character string
		std::string name = str.substr(7, 13);

		// Extract edit counter
		std::string counter = str.substr(21, 3);

		return mac + "_" + name + "_" + counter;
	}
	void timestampToDateTime(uint32_t timestamp, int& day, int& month, int& year, int& hour, int& min, int& sec) {
		const time_t baseTime = 946684800; // 1.1.2000, 00:00:00

		time_t rawTime = baseTime + timestamp;

		std::tm timeInfo;
		gmtime_s(&timeInfo, &rawTime);

		day = timeInfo.tm_mday;
		month = timeInfo.tm_mon + 1;
		year = timeInfo.tm_year + 1900;
		hour = timeInfo.tm_hour;
		min = timeInfo.tm_min;
		sec = timeInfo.tm_sec;
	}



	uint32_t dateTimeToTimestamp(int year, int month, int day, int hour, int minute, int second) {
		// Validate input (optional but recommended)
		if (year < 2000 || month < 1 || month > 12 || day < 1 || day > 31 ||
			hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
			throw std::invalid_argument("Invalid date and time values provided.");
		}

		//This dateTimeToTimestamp is very weirdly made, i dont even know how to explain it, flipnotes addes an extra hour, while saving;
		hour += 1;

		std::tm timeInfo = {};
		timeInfo.tm_sec = second;
		timeInfo.tm_min = minute;
		timeInfo.tm_hour = hour;
		timeInfo.tm_mday = day;

		timeInfo.tm_mon = month - 1;

		timeInfo.tm_year = year - 1900;

		time_t timeSinceEpoch = mktime(&timeInfo);

		time_t baseTime = 946684800;
		uint32_t timestamp = static_cast<uint32_t>(timeSinceEpoch - baseTime);

		return timestamp;
	}
};

namespace Utils
{
	bool fileExists(const std::string& path) {
		std::ifstream file(path);
		return file.good();
	}

	bool ValidateAuthorIDs(HWND hWnd, const char* authorID, const char* fieldName) {
		if (!convert::IsHexadecimal(authorID)) {
			std::string errorMessage = "The ";
			errorMessage += fieldName;
			errorMessage += " must only contain hexadecimal characters: 0123456789ABCDEF";
			MessageBox(hWnd, errorMessage.c_str(), "Error", MB_ICONERROR);
			return false;
		}
		else if (strlen(authorID) != 16) {
			std::string errorMessage = "The ";
			errorMessage += fieldName;
			errorMessage += " must be exactly 16 characters long";
			MessageBox(hWnd, errorMessage.c_str(), "Error", MB_ICONERROR);
			return false;
		}
		return true;
	}

	bool ValidateInput(HWND hWnd, const wchar_t* szInput, int minValue, int maxValue, const char* szErrorMessage)
	{
		int value = std::stoi(szInput);
		if (value < minValue || value > maxValue)
		{
			MessageBox(hWnd, szErrorMessage, "Error", MB_ICONERROR);
			return false;
		}
		return true;
	}
};
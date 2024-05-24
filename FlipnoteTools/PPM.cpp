#include "PPM.hpp"
#include <fstream>
#include <Windows.h>


//validating the ppm file + fixes an old bug
extern HWND hMainWindow;
bool ppm_is_valid(std::ifstream &ppm)
{
	char paraStr[4];
	ppm.seekg(0x0);
	ppm.read(paraStr, sizeof(paraStr));

	if (std::memcmp(paraStr, "PARA", sizeof(paraStr)) != 0)
	{
		MessageBox(hMainWindow, "The selected file is not a valid PPM file.", "Error", MB_ICONERROR);
		ppm.close();
		return false;
	}

	ppm.seekg(0, std::ios::end);
	std::streampos fileSize = ppm.tellg();
	ppm.seekg(0, std::ios::beg);
	
	if (fileSize < 0x730)
	{
		MessageBox(hMainWindow, "The selected file is not a valid PPM file.", "Error", MB_ICONERROR);
		ppm.close();
		return false;
	}
	if (fileSize > 0x100000) //1mb
	{
		MessageBox(hMainWindow, "The PPM file exceeds 1MB in size. Flipnotes larger than 1MB are considered corrupted in Flipnote Studio. Please split the file into separate PPM files.", "File Size Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void PPM::open(const char* path) {
	this->filepath = path;

	std::ifstream file(filepath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error opening file\n";
		return;
	}

	this->is_valid = ppm_is_valid(file);
	if (!this->is_valid) {
		this->isOpen = false;
		file.close();
		return;
	}

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer = new char[fileSize];
	file.read(buffer, fileSize);

	file.seekg(0x10);
	file.read(reinterpret_cast<char*>(&is_locked), sizeof(is_locked));

	file.seekg(0x40);
	file.read(reinterpret_cast<char*>(creator_name), sizeof(creator_name));

	file.seekg(0x14);
	file.read(reinterpret_cast<char*>(original_creator_name), sizeof(original_creator_name));

	file.seekg(0x5E);
	file.read(reinterpret_cast<char*>(&author_id), sizeof(uint64_t));

	file.seekg(0x56);
	file.read(reinterpret_cast<char*>(&original_author_id), sizeof(uint64_t)); 
	
	file.seekg(0x88);
	file.read(reinterpret_cast<char*>(&num_fn), sizeof(sizeof(num_fn)));

	file.seekg(0x9A);
	file.read(reinterpret_cast<char*>(&timestamp), sizeof(uint32_t));

	file.seekg(0x78);
	file.read(mac_fn, sizeof(mac_fn));

	file.seekg(0x7B);
	file.read(rnd_fn, sizeof(rnd_fn));


	this->isOpen = true;
	file.close();
}

std::string PPM::get_filepath()
{
	return this->filepath;
}

bool PPM::get_isOpen()
{
	return this->isOpen;
}

//fixes the Old Version of this Application
void writeWString(std::fstream& file, wchar_t* str, std::streamsize size, std::streampos pos) {
		file.seekp(pos);
		std::wstring nullStr(size, L'\0');
		file.write(reinterpret_cast<const char*>(&nullStr[0]), size);
		file.seekp(pos);
		file.write(reinterpret_cast<char*>(str), wcslen(str) * sizeof(wchar_t));
}

void PPM::save(const char *path)
{
	std::ofstream file(path, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error writing file\n";
		return;
	}

	//saving the Flipnote
	file.write(buffer, fileSize);
	file.close();

	//open the file again to make some changes
	std::fstream file2(path, std::ios::binary | std::ios::in | std::ios::out);
	if (!file2.is_open()) {
		std::cerr << "Error writing file\n";
		return;
	}

		
		file2.seekp(0x10);
		file2.write(reinterpret_cast<const char*>(&is_locked), sizeof(bool));

	
		writeWString(file2, creator_name, 0x14, 0x40);

		
		writeWString(file2, original_creator_name, 0x14, 0x14);

		
		writeWString(file2, original_creator_name, 0x14, 0x2A);

		file2.seekp(0x5E);
		file2.write(reinterpret_cast<const char*>(&author_id), sizeof(uint64_t));

		file2.seekg(0x56);
		file2.write(reinterpret_cast<char*>(&original_author_id), sizeof(uint64_t));

		file2.seekg(0x8A);
		file2.write(reinterpret_cast<char*>(&original_author_id), sizeof(uint64_t));

		file2.seekg(0x9A);
		file2.write(reinterpret_cast<char*>(&timestamp), sizeof(uint32_t));

		//file names:
		file2.seekg(0x78);
		file2.write(reinterpret_cast<char*>(&mac_fn), sizeof(mac_fn));

		//the random section:
		file2.seekg(0x7B);
		file2.write(reinterpret_cast<char*>(rnd_fn), sizeof(rnd_fn));

		//num of edits:
		file2.seekg(0x88);
		file2.write(reinterpret_cast<char*>(&num_fn), sizeof(uint16_t));

		file2.close();
		
		std::string message = "File was successfully saved under: ";
		message += path;
		MessageBox(NULL, message.c_str(), "File", MB_OK | MB_ICONINFORMATION);
}


bool PPM::get_is_valid()
{
	return is_valid;
}

bool PPM::get_is_locked() const {
	return is_locked;
}

void PPM::set_is_locked(bool lock) {
	is_locked = lock;
}

std::wstring PPM::get_creator_name() const {
	return std::wstring(creator_name);
}

void PPM::set_creator_name(const wchar_t* name) {
	wcsncpy_s(creator_name, name, 11);
}

std::wstring PPM::get_original_creator_name() const {
	return std::wstring(original_creator_name);
}

void PPM::set_original_creator_name(const wchar_t* name) {
	wcsncpy_s(original_creator_name, name, 11);
}

//author id's
uint64_t PPM::get_original_author_id() {
	return original_author_id;
}

void PPM::set_original_author_id(const uint64_t id) {
	original_author_id = id;
}

uint64_t PPM::get_author_id() {
	return author_id;
}

void PPM::set_author_id(const uint64_t id) {
	author_id = id;
}

//timestamp
uint32_t PPM::get_timestamp()
{
	return timestamp;
}
void PPM::set_timestamp(const uint32_t time)
{
	timestamp = time;
}

//filename stuff:
void PPM::set_mac_filename(const std::string& name)
{
	if (name.length() < 6) {
		throw std::invalid_argument("Please enter a valid file name.");
	}

	for (int i = 0; i < 3; ++i) {
		if (sscanf_s(name.substr(i * 2, 2).c_str(), "%2hhx", &mac_fn[i]) != 1)
			throw std::invalid_argument("Please enter a valid file name.");
	}
}

bool PPM::set_rnd_filename(const std::string& name) {
	try {
		if (name.length() < 13) {
			throw std::invalid_argument("Please enter a valid file name.");
		}

		std::cout << "name string is: " << name << std::endl;
		if (sscanf_s(name.c_str(), "%13s", rnd_fn) != 1) {
			throw std::invalid_argument("Please enter a valid file name.");
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Exception caught in set_rnd_filename: " << e.what() << std::endl;
		return false;
		throw;
	}
}


bool PPM::set_num_filename(const std::string& num) {
	try {
		if (num.empty()) {
			throw std::invalid_argument("Please enter a valid file name.");
		}

		if (sscanf_s(num.c_str(), "%hd", &num_fn) != 1) {
			throw std::invalid_argument("Please enter a valid file name.");
		}
		return true;
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Exception caught in set_num_filename: " << e.what() << std::endl;
		return false;
	}
}

std::string PPM::get_mac_filename() const
{
	char mac[7];
	snprintf(mac, sizeof(mac), "%02X%02X%02X", static_cast<unsigned char>(mac_fn[0]), static_cast<unsigned char>(mac_fn[1]), static_cast<unsigned char>(mac_fn[2]));
	return std::string(mac);
}

std::string PPM::get_rnd_filename() const
{
	return std::string(rnd_fn, 13);
}

uint16_t PPM::get_num_filename() const
{
	return num_fn;
}
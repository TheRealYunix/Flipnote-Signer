#pragma once
#include <iostream>
#include <cstdint>
#include <string>

class PPM {

private:

	//File stuff:
	std::string filepath;
	char* buffer;
	std::streamsize fileSize;

	bool is_valid;
	bool isOpen = false;
	bool is_locked;

	//Creator/Author names
	wchar_t creator_name[12];
	wchar_t original_creator_name[12];

	//Author ID's
	uint64_t author_id;
	uint64_t original_author_id;

	//Timestamp
	uint32_t timestamp;

	//Filename stuff
	char mac_fn[3];
	char rnd_fn[13];
	uint16_t num_fn;

public:

	PPM() : buffer(nullptr), fileSize(0) {}

	~PPM() {
		delete[] buffer;
	}

	std::string get_filepath();
	bool get_isOpen();
	//Opening the PPM File
	void open(const char* path);
	//In other words "Save"
	void save(const char *path);


	char calc_check_digit(const std::string& filename);
	//IsValid
	bool get_is_valid();

	//Lock
	bool get_is_locked() const;
	void set_is_locked(bool lock);

	//author name
	std::wstring get_creator_name() const;
	void set_creator_name(const wchar_t* name);

	//Original author name
	std::wstring get_original_creator_name() const;
	void set_original_creator_name(const wchar_t* name);

	//author ID
	uint64_t get_author_id();
	void set_author_id(const uint64_t id);

	//Original author ID
	uint64_t get_original_author_id();
	void set_original_author_id(const uint64_t id);

	//Last modified Timestamp
	uint32_t get_timestamp();
	void set_timestamp(const uint32_t timestamp);

	//Filename stuff:
	void set_mac_filename(const std::string& name);
	bool set_rnd_filename(const std::string& name);
	bool set_num_filename(const std::string& num);

	std::string get_mac_filename() const;
	std::string get_rnd_filename() const;
	uint16_t get_num_filename() const;
};

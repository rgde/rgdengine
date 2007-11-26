// archive_builder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "utility.h"
#include "archive.h"



int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring start_path = L".";
	std::string fs_filename = "test.fs";

	using namespace rgde;
	namespace fs = rgde::filesystem;

	fs::file_info_vector files; 
	fs::utility::find_files(start_path, files);

	fs::archive::build(files, L"test.archive");

	fs::archive archive(L"test.archive");

	const fs::archive::archive_entry* entry = archive.find_entry(archive.get_entries()[0].name);

	std::vector<char> data;
	bool is_done = archive.read(entry, 0, entry->size, data);

	boost::filesystem::ifstream in(entry->name, std::ios::binary);
	int test_size = boost::filesystem::file_size(entry->name);
	std::vector<char> test_data(test_size);
	in.read(&test_data.front(), test_size);
	int res = memcmp(&data.front(), &test_data.front(), data.size());

	return 0;
}


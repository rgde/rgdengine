#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::filesystem;

namespace fs
{
	using namespace boost::filesystem;
}

using namespace std;

std::vector<path> files_list;

bool find_file( const path & dir_path/*,     // in this directory,
			   const std::string & file_name*/, // search for this name,
			   path & path_found )        // placing path here if found
{
	if ( !exists( dir_path ) ) return false;
	directory_iterator end_it; // default construction yields past-the-end
	for ( directory_iterator it( dir_path ); it != end_it; ++it )
	{
		if ( is_directory( *it ) )
		{
			if ( find_file( *it, /*file_name, */path_found ) ) return true;
		}
		else //if ( itr->leaf() == file_name ) // see below
		{
			//cout << it->leaf().c_str() << endl; 
			files_list.push_back(*it);
			//path_found = *itr;
			//return true;
		}
	}
	return false;
}

struct file_info
{
	std::string path;
	int size;
	int offset;
};

istream& operator>>(istream& istr, file_info& fi)
{
	istr >> fi.path >> fi.offset >> fi.size;
	return istr;
}

ostream& operator<<(ostream& ostr, const file_info& fi)
{
	ostr.write(fi.path.c_str(), fi.path.size());
	ostr.write((const char*)&(fi.offset), sizeof(int));
	ostr.write((const char*)&(fi.size), sizeof(int));
	int total_size = fi.path.size() + 2 * sizeof(int);
	ostr.write((const char*)&total_size, sizeof(int));
	return ostr;
}


std::vector<file_info> fat;

void buildFS(std::string fs_file, fs::path start_path = ".")
{
	fs::remove(fs_file);

	path p;
	find_file(start_path, p);

	std::ofstream fs(fs_file.c_str(), ios::binary);

	for(unsigned int i = 0; i < files_list.size(); ++i)
	{
		path& p = files_list[i];
		

		int size = fs::file_size(p);
		fs::ifstream in(p, ios::binary);

		std::vector<char> file_data;
		file_data.resize(size);

		in.read(&file_data[0], size);

		fs.write(&file_data[0], size);
		//fs << p;

		file_info fi;
		fi.offset = fat.size() > 0 ? fat[fat.size()-1].offset + fat[fat.size()-1].size: 0;
		fi.size = size;
		fi.path = p.relative_path().string();//.native_file_string();
		fat.push_back(fi);

		//cout << p.leaf().c_str() << endl; 
		cout << fi.path << "  " << fi.offset << "  " << fi.size << endl; 
	}

	for (int i = 0; i < fat.size(); ++i)
	{
		fs << fat[i];
	}

	int total = fat.size() > 0 ? fat[fat.size()-1].offset + fat[fat.size()-1].size: 0;
	fs.write((const char*)&total, sizeof(int));
	//fs.close();
}


void readFS(std::string fs_filename)
{
	int size = fs::file_size(fs_filename);
	std::ifstream fs(fs_filename.c_str(), ios::binary);

	fs.seekg(size - sizeof(int));
	int data_size = 0;
	fs.read((char*)&data_size, sizeof(int));

	int fat_size = size - data_size - sizeof(int);

	int fat_start = data_size;

	bool stopReadingFat = false;
	int curOffSet = sizeof(int)*2;
	
	while(!stopReadingFat)
	{
		fs.seekg(size - curOffSet);
		int fi_size = 0;
		fs.read((char*)&fi_size, sizeof(int));

		std::vector<char> fi_data;
		fi_data.resize(fi_size);

		fs.seekg(size - curOffSet - fi_size);
		fs.read((char*)&(fi_data[0]), fi_size);

		curOffSet += fi_size + sizeof(int);

		file_info fi;
		fi.path = std::string(&(fi_data[0]), fi_data.size() - sizeof(int)*2);
		fi.size = *(int*)(&fi_data[fi_data.size() - sizeof(int)]);
		fi.offset = *(int*)(&fi_data[fi_data.size() - sizeof(int)*2]);

		fat.push_back(fi);

		cout << fi.path << "  " << fi.offset << "  " << fi.size << endl; 

		if (fi.offset == 0)
			break;
	}
}

void main(int argc, char *argv[] )
{
	//string start_path = ".";
	//string fs_filename = "test.fs";

	//if (argc > 1)
	//	start_path = argv[1];

	//if (argc > 2)
	//	fs_filename = argv[2];

	//buildFS(fs_filename, start_path + "/");
	//cout << "-----------------------------------" << endl;
	//readFS(fs_filename);

	readFS(argv[1]);


	return;
}
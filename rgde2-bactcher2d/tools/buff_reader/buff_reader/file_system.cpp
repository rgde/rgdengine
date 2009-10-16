#include "stdafx.h"
#include "file_system.h"

const std::string exclude_dir = ".svn";
namespace fs = boost::filesystem;

const std::string exclude_files_mask = "*.bmp;*.x";

bool FindMedia( std::string& file_name )
{
	return file_system::instance().get(file_name);
}

file_system& file_system::instance()
{
	static file_system instance;
	return instance;
}

void file_system::clear()
{
	map_.clear();
}

bool file_system::get( std::string& file_name )
{
	file_map::const_iterator found = map_.find(file_name);
	if( found != map_.end() )
	{
		file_name = found->second[0];
		return true;
	}
	else
		return false;
}

void file_system::add_path( const std::string& dir_path )
{
	fs::path p = fs::system_complete( fs::path( dir_path, fs::native ) );
	if( fs::exists( p ) )
		enumerate( p, map_ );
	else
		std::cout << "\nNot found: " << p.native_file_string() << std::endl;		
}

void file_system::to_own( std::string& file_name )
{
	fs::path p = fs::system_complete( fs::path( file_name, fs::native ) );
	file_name = p.string();
}

void file_system::enumerate( const fs::path& dir_path, file_map& _map  )
{
	if ( !fs::exists( dir_path ) )
		return;

	fs::directory_iterator end_itr; // default construction yields past-the-end
	for( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		fs::path path = itr->path();
		if( fs::is_directory(path) )
		{
			if( path.leaf() != exclude_dir )
				enumerate( *itr, _map );
		}
		else
		{
			std::string short_name = path.leaf();

			_map[short_name].push_back(path.root_path().string()+path.relative_path().string());
		}
	}
}

void file_system::dump_dups()
{
	for (map_citer it = map_.begin(); it != map_.end(); ++it)
	{
		const std::vector<std::string> files_list = it->second;

		if (files_list.size() > 1)
		{
			std::cout << "=======================================================================" << std::endl;
			std::cout << "File dups found for file: '" << it->first.c_str() << "' !!!" << std::endl;
			for (size_t i = 0; i < files_list.size(); ++i)
			{
				std::cout << files_list[i].c_str() << std::endl;
			}
		}
	}

	std::cout << "=======================================================================" << std::endl;
}


bool find_file( const fs::path & dir_path,		// in this directory,
			   const std::string & file_name,	// search for this name,
			   fs::path & path_found )			// placing path here if found
{
	if ( !fs::exists( dir_path ) )
		return false;

	fs::directory_iterator end_itr; // default construction yields past-the-end
	for( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		fs::path path = itr->path();

		if( fs::is_directory(*itr) )
		{
			if ( find_file( *itr, file_name, path_found ) ) return true;
		}
		else if ( path.leaf() == file_name ) // see below
		{
			path_found = *itr;
			return true;
		}
	}
	return false;
}
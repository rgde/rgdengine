#include "stdafx.h"

#include "utility.h"

namespace rgde
{
	namespace filesystem
	{
		namespace utility
		{
			bool find_files(const path& dir_path, file_info_vector& files_list)
			{
				using namespace boost::filesystem;

				if ( !exists( dir_path ) )
					return false;

				// default construction yields past-the-end
				wdirectory_iterator end_it;

				for ( wdirectory_iterator it( dir_path ); it != end_it; ++it )
				{
					if ( is_directory( *it ) )
					{
						if ( find_files( *it, files_list) ) return true;
					}
					else
					{						
						wpath p = *it;
						p.normalize();

						file_info info;
						info.name = p.string();
						info.size = (size_t)file_size(p);
						info.last_write_time = last_write_time(p);

						std::wstring time = info.get_time_string();

						files_list.push_back(info);
					}
				}
				return false;
			}
		}
	}
}
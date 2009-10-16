#pragma once

namespace rgde
{
	namespace filesystem
	{
		typedef boost::filesystem::wpath path;

		struct file_info
		{
			std::wstring name;
			size_t		 size;
			std::time_t  last_write_time;

			std::wstring get_time_string() const;
		};

		typedef std::vector<file_info> file_info_vector;
	}
}
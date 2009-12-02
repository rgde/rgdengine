#include "stdafx.h"

#include <rgde/core/file_system.h>

namespace rgde
{
	namespace core
	{
		namespace vfs
		{
			std::wstring get_extension(const std::wstring& file_name)
			{
				size_t pos = file_name.rfind(L".");

				if(pos == std::wstring::npos) 
					return std::wstring();

				return file_name.substr(pos, file_name.size() - pos - 1);
			}

			std::wstring strip_extension(const std::wstring& file_name)
			{
				size_t pos	= file_name.rfind(L".");

				if(pos == std::wstring::npos)
					return file_name;
				else
					return file_name.substr(0, pos);
			}

			std::string get_extension(const std::string& file_name)
			{
				size_t pos = file_name.rfind(".");
				
				if(pos == std::string::npos) 
					return std::string();

				return file_name.substr(pos, file_name.size() - pos - 1);
			}

			std::string strip_extension(const std::string& file_name)
			{
				size_t pos	= file_name.rfind(".");

				if(pos == std::string::npos)
					return file_name;
				else
					return file_name.substr(0, pos);
			}

			void system::add_file_source(file_source_ptr fs)
			{
				m_sources.push_back(fs);
			}

			istream_ptr system::open_read(const std::string& file_name)
			{
				for (src_it it = m_sources.begin(); it != m_sources.end(); ++it)
				{
					if (istream_ptr s = (*it)->open_read(file_name))
						return s;
				}
				return istream_ptr();
			}

			ostream_ptr system::open_write(const std::string& filename)
			{
				return ostream_ptr();
			}

			bool system::is_file_exist(const std::string& file_name)
			{
				for (src_it it = m_sources.begin(); it != m_sources.end(); ++it)
				{
					if ((*it)->is_file_exist(file_name))
						return true;
				}
				return false;
			}
		}
	}
}
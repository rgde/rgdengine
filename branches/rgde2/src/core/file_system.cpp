#include "stdafx.h"

#include <rgde/core/file_system.h>

#include <windows.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

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

			//////////////////////////////////////////////////////////////////////////
			namespace internal
			{
				namespace fs = boost::filesystem;

				struct disk_istream : public istream
				{
					void open(const fs::wpath& p)
					{
						m_file_size = fs::file_size( p );
						m_file.open(p, std::ios::binary);
					}

					size_t read(byte* buff, unsigned size)
					{
						m_file.read((char*)buff, size);
						return m_file.gcount();
					}

					size_t get_size() const
					{
						return m_file_size;
					}

					void set_pos(size_t pos) 
					{
						m_file.seekg((std::streamoff)pos);
					}

					bool is_valid() const {return m_file.good();}
					size_t get_pos() const {return m_file.tellg();}

					size_t m_file_size;
					mutable fs::ifstream m_file;

				};

				struct disk_ostream : public ostream
				{
					void open(const fs::wpath& p)
					{
						m_file.open(p, std::ios::binary);
					}

					void write(const byte* buff, unsigned size)
					{
						m_file.write((const char*)buff, size);
					}

					bool is_valid() const 
					{
						return m_file.good();
					}

					size_t get_pos() const 
					{
						return m_file.tellp();
					}

					mutable fs::ofstream m_file;
				};


				struct disk_file_source : public file_source
				{
					disk_file_source() 
					{
						wchar_t buf[512];
						GetModuleFileNameW(NULL, &buf[0], 512);

						fs::wpath p(buf);
						std::wstring path = p.branch_path().string() + L"/../data/";

						m_root_read_path = path;
						m_root_write_path = path;

						fs::current_path(m_root_read_path);
					}

					~disk_file_source() {}

					virtual int get_priority() const {return 0;}

					virtual bool is_support_write() const {return true;}

					fs::wpath get_file_path(const std::string& filename) const
					{
						std::wstring temp_str(filename.begin(), filename.end());
						fs::wpath out = m_root_read_path / fs::wpath (temp_str);
						out.normalize();
						return out;
					}

					virtual bool is_file_exist(const std::string& file_name) const 
					{
						fs::wpath p = get_file_path(file_name);
						bool exist = fs::exists(p);
						bool regular = fs::is_regular_file(p);
						return exist && regular;
					}

					virtual istream_ptr open_read(const std::string& file_name)
					{
						if (!is_file_exist(file_name))
							return istream_ptr();

						disk_istream *f  = new disk_istream();
						f->open(get_file_path(file_name));
						return istream_ptr(f);
					}

					virtual ostream_ptr open_write(const std::string& file_name)
					{
						disk_ostream *f  = new disk_ostream();
						f->open(get_file_path(file_name));
						return ostream_ptr(f);
					}

					fs::wpath m_root_read_path;
					fs::wpath m_root_write_path;
				};
			}
			//////////////////////////////////////////////////////////////////////////
			system::system()
			{
				add_file_source(file_source_ptr(new internal::disk_file_source));
			}

			system::~system()
			{
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
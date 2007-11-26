#include "stdafx.h"

#include "archive.h"

#include <boost/filesystem/fstream.hpp>

//////////////////////////////////////////////////////////////////////////
// Archive file format:
// ^^^^^^^^^^^^^^^^^^^
// 0 ... n bytes - any data
// -----------------------------------------------------------------------
// --> start 
// ... of files data
// <-- end
// --> start 
// ... of descriptors data
// <-- end
// sizeof(size_t) - size of files data block
// sizeof(size_t) - size of descriptors data block
// sizeof(int) - arhive format version XXXX
// sizeof(end_of_archive_mark) - end of archive mark 
// EOF
//////////////////////////////////////////////////////////////////////////

namespace rgde
{
	namespace filesystem
	{
		namespace details
		{
			static const char start_of_archive_mark[] = "##start_of_arhive##";
			static const char end_of_archive_mark[] = "##end_of_arhive##";

			size_t archive_header_size()
			{
				return sizeof(end_of_archive_mark) + sizeof(int) + sizeof(size_t)*2;
			}
		

			size_t write(boost::filesystem::ofstream& out , const file_info& finfo)
			{
				int str_bytes = (int)(sizeof(wchar_t)*(finfo.name.size()+1));
				out.write((const char*)&str_bytes, sizeof(str_bytes));
				out.write((const char*)finfo.name.c_str(), str_bytes);
				out.write((const char*)&finfo.size, sizeof(finfo.size));
				out.write((const char*)&finfo.last_write_time, sizeof(finfo.last_write_time));
				
				return str_bytes + sizeof(int) + sizeof(finfo.size) + sizeof(finfo.last_write_time);
			}

			size_t read(boost::filesystem::ifstream& in , file_info& finfo)
			{
				size_t readed_bytes = 0;
				int str_bytes = 0;
				in.read((char*)&str_bytes, sizeof(str_bytes));
				readed_bytes += sizeof(str_bytes);
				std::vector<char> str_data(str_bytes);
				in.read((char*)&str_data.front(), str_bytes);
				readed_bytes += str_bytes;

				finfo.name = std::wstring((const wchar_t*)&str_data.front());

				in.read((char*)&finfo.size, sizeof(finfo.size));
				readed_bytes += sizeof(finfo.size);
				in.read((char*)&finfo.last_write_time, sizeof(finfo.last_write_time));
				readed_bytes += sizeof(finfo.last_write_time);
				return readed_bytes;
			}
		}

		bool operator < (const archive::archive_entry& lhs, const archive::archive_entry& rhs)
		{
			return lhs.name < rhs.name;
		}

		bool operator==(const archive::archive_entry& lhs, const std::wstring& rhs)
		{
			return lhs.name == rhs;
		}

		bool operator==(const archive::archive_entry& lhs, const path& rhs)
		{
			return lhs.name == rhs.string();
		}
		archive::archive(const path& archive_file)
		{
			open(archive_file);
		}

		archive::archive()
		{

		}

		bool archive::open(const path& archive_file_path)
		{
			namespace fs = boost::filesystem;

			m_entries.clear();			

			if (!fs::exists(archive_file_path))
				return false;

			
			//fs::ifstream archive_file(archive_file_path, std::ios::binary);
			m_archive_stream.open(archive_file_path, std::ios::binary);
			fs::ifstream& archive_file = m_archive_stream;

			size_t size = fs::file_size(archive_file_path);
			
			{
				const size_t data_size = sizeof(details::end_of_archive_mark);
				archive_file.seekg(size - data_size);
				std::vector<char> data(data_size);
				archive_file.read(&data.front(), data_size);
				bool identical = 0 == memcmp(&data.front(), details::end_of_archive_mark, data_size);
				if (!identical)
				{
					std::cerr << "Error: archive::open(...) - invalid archive format!" << std::endl;
					return false;
				}
			}

			int version = 0;
			archive_file.seekg(size - sizeof(details::end_of_archive_mark) - sizeof(int));
			archive_file.read((char*)&version, sizeof(int));

			size_t descriptors_data_size = 0;
			archive_file.seekg(size - details::archive_header_size() + sizeof(size_t));
			archive_file.read((char*)&descriptors_data_size, sizeof(size_t));

			const size_t start_of_descriptors_data = size - details::archive_header_size() - descriptors_data_size;

			size_t files_data_size = 0;
			archive_file.seekg(size - details::archive_header_size());
			archive_file.read((char*)&files_data_size, sizeof(size_t));

			const size_t start_of_files_data = start_of_descriptors_data - files_data_size;

			{
				size_t data_size = sizeof(details::start_of_archive_mark);
				std::vector<char> data(data_size);
				archive_file.seekg(start_of_files_data - data_size);
				archive_file.read(&data.front(), data_size);
				bool identical = 0 == memcmp(&data.front(), details::start_of_archive_mark, data_size);
				if (!identical)
				{
					std::cerr << "Error: archive::open(...) - invalid archive format!" << std::endl;
					return false;
				}
			}

			std::cout << "data size: " << files_data_size << std::endl;
			std::cout << "desc size: " << descriptors_data_size << std::endl;

			int size_to_read = descriptors_data_size;
			archive_file.seekg(start_of_descriptors_data);
			size_t cur_offset = start_of_files_data;
			for(;size_to_read > 0;)
			{
				archive_entry entry;
				size_to_read -= details::read(archive_file, entry);
				entry.offset = cur_offset;
				m_entries.push_back(entry);
				cur_offset += entry.size;
			}

			std::sort(m_entries.begin(), m_entries.end());

			return is_loaded();
		}

		bool archive::read(const archive::archive_entry* entry, size_t offset, 
							size_t data_size, std::vector<char>& out_data) const
		{
			if (!is_loaded() || NULL == entry || offset > data_size || data_size > entry->size || 
				data_size + offset > entry->size)
				return false;

			out_data.clear();

			out_data.resize(data_size);
			m_archive_stream.seekg(entry->offset + offset);
			m_archive_stream.read(&out_data.front(), data_size);
			return true;
		}

		bool archive::build(const file_info_vector& files, const path& out_arhive_file)
		{
			namespace fs = boost::filesystem;

			fs::remove(out_arhive_file);
			
			fs::ofstream archive_file(out_arhive_file, std::ios::binary);

			if (!fs::exists(out_arhive_file))
			{
				std::cerr << "Error: archive::build(...) : Can't creating archive file!" << std::endl;
				return false;
			}

			archive_file.write(details::start_of_archive_mark, sizeof(details::start_of_archive_mark));
			size_t total_data_size = sizeof(details::start_of_archive_mark);

			typedef file_info_vector::const_iterator iter;
			iter begin = files.begin(), end = files.end();

			path complete_archive_path = fs::complete(out_arhive_file);
			complete_archive_path.normalize();
			
			for(iter it = begin; it != end; ++it)
			{
				const file_info& finfo = *it;

				{	// we need skip our archive file itself :)
					path complete_path = fs::complete(finfo.name);
					complete_path.normalize();

					if (complete_archive_path == complete_path)
						continue;
				}

				fs::ifstream file(finfo.name, std::ios::binary);
				if(fs::exists(finfo.name)) // for std stuff workaround
				{
					size_t size = finfo.size;
					std::vector<char> data(size);
					file.read(&data.front(), (std::streamsize)size);

					archive_file.write(&data.front(), (std::streamsize)size);
					total_data_size += size;
				}
			}

			size_t data_block_size = total_data_size - sizeof(details::start_of_archive_mark);

			for(iter it = begin; it != end; ++it)
			{
				total_data_size += details::write(archive_file, *it);
			}

			size_t descriptors_block_size = total_data_size - data_block_size 
											- sizeof(details::start_of_archive_mark);
			
			int version = (int)archive_file_version;

			archive_file.write((const char*)&data_block_size, sizeof(size_t));
			archive_file.write((const char*)&descriptors_block_size, sizeof(size_t));
			archive_file.write((const char*)&version, sizeof(int));
			archive_file.write(details::end_of_archive_mark, sizeof(details::end_of_archive_mark));

			archive_file.close();

			std::cout << "data size: " << data_block_size << std::endl;
			std::cout << "desc size: " << descriptors_block_size << std::endl;
			std::cout << "saved data size: " << total_data_size << std::endl;
			std::cout << "saved file size: " << fs::file_size(out_arhive_file) << std::endl;
			
			return total_data_size > 0;
		}

		struct search_by_string
		{
			bool operator()(const const archive::archive_entry& lhs, const archive::archive_entry& rhs)
			{
				if (lhs.name == rhs.name && 0 == rhs.offset)
				{
					__asm nop;
					(*m_entry) = &lhs;
				}
				return lhs.name < rhs.name;
			}
			const archive::archive_entry** m_entry;
		};

		const archive::archive_entry* archive::find_entry(const std::wstring& file_name) const
		{
			archive_entry entry;
			entry.offset = 0;
			entry.size = 0;
			entry.name = file_name;
			search_by_string pred;
			const archive::archive_entry* search_result = NULL;
			pred.m_entry = &search_result;
			std::binary_search(m_entries.begin(), m_entries.end(), entry, pred);
			return search_result;
		}

		const archive::archive_entry* archive::find_entry(const path& file_path) const
		{
			archive_entry entry;
			entry.offset = 0;
			entry.size = 0;
			entry.name = file_path.string();
			search_by_string pred;
			const archive::archive_entry* search_result = NULL;
			pred.m_entry = &search_result;
			std::binary_search(m_entries.begin(), m_entries.end(), entry, pred);
			return search_result;
		}
	}
}

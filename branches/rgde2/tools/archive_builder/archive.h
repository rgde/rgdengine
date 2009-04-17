#pragma once

#include "types.h"

namespace rgde
{
	namespace filesystem
	{
		class archive
		{
		public:
			enum {archive_file_version = 1000};

			struct archive_entry : public file_info
			{
				size_t offset;				
			};

			typedef std::vector<archive_entry> entries_vector;
			typedef entries_vector::const_iterator entry_iter;

			/// creates archive from file
			explicit archive(const path& archive_file);
			archive();

			bool open(const path& archive_file);

			const path& get_archive_file_path() const {return m_archive_file;}
			bool is_loaded() const					  {return !m_entries.empty();}

			static bool build(const path& dir_path, const path& out_arhive_file);

			const entries_vector& get_entries() const {return m_entries;}

			const archive_entry* find_entry(const std::wstring& file_name) const;
			const archive_entry* find_entry(const path& file_path) const;

			bool read(const archive_entry* entry, size_t offset, size_t data_size, std::vector<char>& out_data) const;

		private:
			entries_vector m_entries;
			path		   m_archive_file;
			mutable boost::filesystem::ifstream m_archive_stream;
		};

		bool operator < (const archive::archive_entry& lhs, const archive::archive_entry& rhs);
	}
}
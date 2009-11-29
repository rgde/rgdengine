#pragma once

#include <rgde/core/file_system_forward.h>
#include <rgde/core/types.h>

namespace rgde
{
	namespace core
	{
		namespace vfs
		{
			struct stream
			{
				virtual ~stream(){}
				virtual bool is_valid() const = 0;
				virtual size_t get_pos() const = 0;
			};

			struct read_stream : public stream
			{
				virtual void read(byte* buff, unsigned size) = 0;

				virtual size_t get_size() const = 0;
				virtual void set_pos(size_t pos) = 0;
			};

			template<typename T>
			read_stream_ptr& operator >> (read_stream_ptr& in_stream, T& value)
			{
				if (in_stream)
					in_stream->read((byte*)&var, sizeof(T));
				return in_stream;
			}

			struct write_stream : public stream
			{
				virtual void write(const byte* buff, unsigned size) = 0;
			};

			template<typename T>
			write_stream_ptr& operator << (write_stream_ptr& out_stream, const T& value)
			{
				if (out_stream)
					out_stream->write((const byte*)&var, sizeof(T));
				return out_stream;
			}
			
			struct file_source
			{
				virtual ~file_source() {}
				virtual int get_priority() = 0;

				virtual bool is_support_write() = 0;
				
				virtual bool is_file_exist(const std::string& file_name) = 0;
				virtual read_stream_ptr open_read(const std::string& file_name) = 0;
			};

			typedef boost::shared_ptr<file_source> file_source_ptr;
			

			class system
			{
			public:
				system();
				~system();

				void add_file_source(file_source_ptr fs);

				read_stream_ptr open_read(const std::string& file_name);
				bool is_file_exist(const std::string& file_name);

			private:
				typedef std::vector<file_source_ptr> sources;
				typedef sources::iterator src_it;
				sources m_sources;
			};
		}
	}
}
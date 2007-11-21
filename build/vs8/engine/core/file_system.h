#pragma once

#include <engine/core/file_system_forward.h>
#include <engine/core/types.h>

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
		}
	}
}
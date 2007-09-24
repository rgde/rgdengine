#pragma once

namespace rgde
{
	namespace core
	{
		namespace vfs
		{
			class file_system;
			class mount_point;

			class path;

			struct stream;
			typedef boost::shared_ptr<stream> stream_ptr;
			
			struct read_stream;
			typedef boost::shared_ptr<read_stream> read_stream_ptr;

			struct write_stream;
			typedef boost::shared_ptr<write_stream> write_stream_ptr;

			std::wstring get_extension(const std::wstring& file_name);
			std::wstring strip_extension(const std::wstring& file_name);
		}
	}
}
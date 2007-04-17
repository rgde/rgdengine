#include "stdafx.h"
#include "file_system.h"

namespace rgde
{
	namespace core
	{
		namespace vfs
		{
			std::wstring get_extension(const std::wstring& file_name)
			{
				int pos = file_name.rfind(L".");
				if(pos == -1) return std::wstring();

				return file_name.substr(pos, file_name.size() - pos - 1);
			}

			std::wstring strip_extension(const std::wstring& file_name)
			{
				int pos	= file_name.rfind(L".");

				if(pos == -1)
					return file_name;
				else
					return file_name.substr(0, pos);
			}
		}
	}
}
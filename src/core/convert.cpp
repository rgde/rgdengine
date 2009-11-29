#include "stdafx.h"
#include <rgde/core/convert.h>

namespace rgde
{
	namespace core
	{
		namespace utils
		{
			std::string convert2str(const std::wstring &wstr)
			{
				return std::string(wstr.begin(), wstr.end());
			}

			std::wstring convert2wstr(const std::string &str)
			{
				return std::wstring(str.begin(), str.end());
			}
		}
	}
}
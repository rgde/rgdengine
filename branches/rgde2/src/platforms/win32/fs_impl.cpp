#include "stdafx.h"

#include <rgde/core/file_system.h>
#include <boost/filesystem.hpp>

namespace rgde
{
	namespace core
	{
		namespace vfs{
			namespace internal
			{
				namespace fs = boost::filesystem;
				extern fs::wpath get_root_path();

				fs::wpath get_root_path() {
					wchar_t buf[512];
					GetModuleFileNameW(NULL, &buf[0], 512);
					fs::wpath p(buf);
					return p.branch_path();
				}
			}
		}
	}
}
#pragma once

#include "types.h"

namespace rgde
{
	namespace filesystem
	{
		namespace utility
		{
			/// returns true is any file was found
			bool find_files(const path& start_dir, file_info_vector& search_result);
		}

	}
}
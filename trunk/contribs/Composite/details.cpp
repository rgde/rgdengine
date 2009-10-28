#include "stdafx.h"

#include "type_info.h"

namespace meta
{
	namespace details
	{
		void factory::register_type(type_information& ti)
		{
			types[ti.type_name] = &ti;
		}

		std::string get_type_name(const type_info& info)
		{
			std::string type_name = info.name();

			typedef std::string::size_type size_type;

			//TODO: handle case of other than MSVC compilers
			static const std::string msvc_typeid_prefix = "class ";
			size_type pos = type_name.find(msvc_typeid_prefix);

			if (std::string::npos != pos )
			{
				size_type start_pos = pos + msvc_typeid_prefix.size();
				type_name = std::string(type_name, start_pos, type_name.size() - start_pos);
			}

			return type_name;
		}
	}
}
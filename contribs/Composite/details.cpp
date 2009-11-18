#include "stdafx.h"

#include "type_info.h"

namespace meta
{
	namespace details
	{
		abstract_property::abstract_property(const string& _name, const type_info& owner, const type_info& type)
			: value_type(type)
			, owner_type(owner)
			, name(_name)
		{
		}

		abstract_property::~abstract_property() 
		{
		}

		bool abstract_property::operator==(const abstract_property& p) const 
		{
			return name == p.name;
		}

		bool abstract_property::operator!=(const abstract_property& p) const 
		{
			return !((*this)==p);
		}

		//////////////////////////////////////////////////////////////////////////

		bool operator==(const property_ptr& p1, const property_ptr& p2)
		{
			return *p1 == *p2;
		}

		bool operator!=(const property_ptr& p1, const property_ptr& p2)
		{
			return *p1 != *p2;
		}

		//////////////////////////////////////////////////////////////////////////


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
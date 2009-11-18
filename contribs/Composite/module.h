#pragma once

#include "type_info.h"
#include "object.h"
#include "concrete_type_info.h"

namespace meta
{
	struct module
	{
		types_map types;
		std::string name;

		module(){}

		explicit module(const std::string& module_name) 
			: name(module_name) 
		{}

		object_ptr create(const std::string& type_name)
		{
			return object_ptr();
		}

		template<class T>
		details::concrete_type_information<T>& def_class(const std::string& name)
		{
			typedef details::concrete_type_information<T> ti;
			//typedef boost::shared_ptr<typename ti> ti_ptr;

			ti* pti = new ti(name);
			//ti_ptr t(ti);
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			type_information_ptr t(pti);
			types[name] = t;
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return *pti;
		}

		/// для тех кто привык с синтаксису boost::python or lua_bind
		template<class T>
		details::concrete_type_information<T>& class_(const std::string& name)
		{
			return def_class<T>(name);
		}
	};

	typedef boost::shared_ptr<module> module_ptr;
}
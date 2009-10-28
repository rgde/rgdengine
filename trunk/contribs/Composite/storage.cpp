#include "stdafx.h"

#include "storage.h"

namespace meta
{
	module& storage::get_module(const std::string& module_name)
	{
		modules_iter it = modules.find(module_name);
		if (it != modules.end())
			return it->second;

		module m(module_name);
		modules[module_name] = m;

		return modules.find(module_name)->second;
	}
}
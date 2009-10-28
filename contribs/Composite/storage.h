#pragma once

namespace meta
{
	struct storage
	{
		module& get_module(const std::string& module_name);

	private:
		typedef std::map<std::string, module> modules_map;
		typedef modules_map::iterator modules_iter;

		modules_map modules;
	};
}
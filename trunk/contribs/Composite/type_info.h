#pragma once


namespace meta
{
	struct type_information
	{
		const type_info& info;
		const std::string type_name;
		details::factory::types_map parrent_types;

		/// name = custom name. may differ from real type name.
		type_information(const type_info& i, const std::string& name) 
			: info(i), type_name(name){}

		virtual ~type_information(){}
	};

	typedef boost::shared_ptr<struct type_information> type_information_ptr;
	typedef type_information_ptr type;

	//typedef std::list<property_ptr> properties_list;
	//typedef properties_list::iterator iter;

	typedef std::map<std::string, type_information_ptr> types_map;
	typedef types_map::iterator types_map_iter;
}
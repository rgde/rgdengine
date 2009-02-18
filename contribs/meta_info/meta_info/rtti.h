#pragma once

#include "meta.h"

namespace meta
{
	typedef boost::shared_ptr<struct type_information> type_information_ptr;

	typedef std::list<property_ptr> properties_list;
	typedef properties_list::iterator iter;

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

	template<class T>
	std::string get_type_name(const char* t_name = 0)
	{
		const type_info& info = typeid(T);
		std::string type_name = (0 != t_name) ? t_name : get_type_name(info);
		return type_name;
	}
	
	typedef std::map<std::string, type_information_ptr> types_map;
	typedef types_map::iterator types_map_iter;

	struct type_information
	{
		const std::string type_name;
		const type_info& owner_type;
		property_storage local_properties;

		types_map parrent_types;
		//types_map child_types;
		
		//TODO: possible need to do such update only once, since types tree are static
		std::list<property_ptr> get_all_properties()
		{
			typedef std::back_insert_iterator<properties_list> back_ins_iter;

			properties_list total_properties;

			std::copy(local_properties.properties.begin(), local_properties.properties.end(), back_ins_iter(total_properties));	

			for (types_map_iter it = parrent_types.begin(); it != parrent_types.end(); ++it)
			{
				type_information_ptr& ti = it->second;
				properties_list parent_properties = ti->get_all_properties();				
				std::copy(parent_properties.begin(), parent_properties.end(), back_ins_iter(total_properties));				
			}
						
			return total_properties;
		}


		property_ptr find_property(const string& name)
		{
			typedef std::list<property_ptr> properties_list;
			typedef properties_list::iterator prop_iter;

			properties_list properties = get_all_properties();

			prop_iter beg = properties.begin();
			prop_iter end = properties.end();
			for (prop_iter it = beg; it != end; ++it)
			{
				property_ptr p = *it;
				const string& prop_name = p->name;
				if (name == prop_name)
					return p;
			}

			return property_ptr();
		}

		template<class T>
		type_information_ptr create_child_type(const char* t_name = 0)
		{
			type_information_ptr t = create<T>(name);
			add_child_type(t);
			return t;
		}

		void add_parent_type(type_information_ptr t)
		{
			add_type(parrent_types, t);
		}

		type_information(const type_info& ti, const std::string& name) 
			: owner_type(ti), 
			  type_name(name)
		{}

	protected:
		void add_type(types_map& types, const type_information_ptr& ti)
		{
			if (ti && types.find(ti->type_name) == types.end())
				types[ti->type_name] = ti;
		}
	};

	typedef boost::shared_ptr<struct object> object_ptr;

	struct property_holder
	{
		property_holder() {}

		property_holder(struct object* obj, property_ptr property)
		{
		}
	};

	struct object
	{
		virtual ~object() {}
		virtual type_information_ptr get_type_info(const class types_info_storage& storage) = 0;

		//virtual property_holder get_property(const string& name)
		//{
		//	//get_type_info();
		//	property_ptr p = m_type_info->find_property(name);

		//	if (p)
		//	{

		//	}

		//	return 
		//}

	protected:
		// to speed up request, we will cash it here.
		type_information_ptr m_type_info;
	};
	

	namespace details
	{
		template<class T>
		struct ttype_information : public type_information
		{
			static bool created;
			static type_information* instance;			
		};

		template<class T>
		bool ttype_information<T>::created = false;

		template<class T>
		type_information* ttype_information<T>::instance = 0;
	}


}

#define SUPPORT_RTTI(X) virtual meta::type_information_ptr get_type_info(const meta::types_info_storage& storage) {return storage.find_type<X>();}
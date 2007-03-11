// Compostite.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

//#include "bind_test_stuff.h"
//#include "stl_helpers.h"

//#include "composite.h"

namespace meta
{
	namespace details
	{
		typedef std::wstring string;

		struct abstract_property
		{
			const type_info& value_type;
			const type_info& owner_type;

			string name;

			abstract_property(const string& _name, const type_info& owner, 
				const type_info& type)
				: value_type(type),
				owner_type(owner),
				name(_name)
			{
			}

			virtual ~abstract_property() {}

			virtual bool can_write() const = 0;
			virtual bool can_read() const = 0;

			bool operator==(const abstract_property& p) const {return name == p.name;}
			bool operator!=(const abstract_property& p) const {return !((*this)==p);}
		};

		typedef boost::shared_ptr<abstract_property> property_ptr;

		bool operator==(const property_ptr& p1, const property_ptr& p2) { return *p1 == *p2; }
		bool operator!=(const property_ptr& p1, const property_ptr& p2) { return *p1 != *p2; }

		template<class OwnerType, class ValueType>
		struct property : public abstract_property
		{
			typedef OwnerType owner;
			typedef ValueType value_type;

			typedef boost::function<void (owner* _owner, value_type value)> setter_func;
			typedef boost::function<value_type  (owner* _owner )>			getter_func;

			getter_func getter; 
			setter_func setter;

			virtual bool can_write() const {return setter;}
			virtual bool can_read() const {return getter;}

			property(const string& _name, getter_func _getter = getter_func(), setter_func _setter = setter_func())
				: getter(_getter), setter(_setter), abstract_property(_name, typeid(OwnerType),typeid(ValueType))
			{
			}

			template<class K>
			K get(owner* _owner) 
			{
				value_type value = getter(_owner);
				return boost::lexical_cast<K,value_type>(value);
			}

			template<class K>
			bool set(owner* _owner, K new_value) 
			{
				if (!setter) return false;

				try{
					value_type _value = boost::lexical_cast<value_type, K>(new_value);
					setter(_owner, _value);
					return true;
				}
				catch(...)
				{
					return false;
				}
			}
		};

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

	typedef boost::shared_ptr<struct type_information> type_information_ptr;

	//typedef std::list<property_ptr> properties_list;
	//typedef properties_list::iterator iter;

	typedef std::map<std::string, type_information_ptr> types_map;
	typedef types_map::iterator types_map_iter;



	struct type_information
	{
		const type_info& info;
		const std::string type_name;
		types_map parrent_types;

		/// name = custom name. may differ from real type name.
		type_information(const type_info& i, const std::string& name) 
			: info(i), type_name(name){}

		virtual ~type_information(){}



	};


	namespace details
	{
		template<class T>
		struct concrete_type_information : public type_information
		{
			//typename concrete_type_information<T> me__;

			explicit concrete_type_information(const std::string& name)
				: type_information(typeid(T), name)
			{
			}

			//std::string test::*

			template<typename V>
			concrete_type_information& def(const std::string name, V T::* data_member_pointer)
			{
				return *this;
			}

			template<typename V>
			concrete_type_information& def_readonly(const std::string name, V T::* data_member_pointer)
			{
				return *this;
			}

			template<typename V>
			concrete_type_information& def_property(const std::string name, V T::* data_member_pointer)
			{
				return def<V>(name, data_member_pointer);
			}
		};
	}


	struct object{};
	typedef boost::shared_ptr<object> object_ptr;

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
			typedef boost::shared_ptr<ti> ti_ptr;

			ti_ptr t(new ti(name));
			types[name] = t;
			return *t;
		}

		/// для тех кто привык с синтаксису boost::python or lua_bind
		template<class T>
		details::concrete_type_information<T>& class_(const std::string& name)
		{
			return def_class<T>(name);
		}
	};

	typedef boost::shared_ptr<module> module_ptr;

	struct storage
	{
		typedef std::map<std::string, module> modules_map;
		typedef modules_map::iterator modules_iter;

		modules_map modules;

		module& get_module(const std::string& module_name)
		{
			modules_iter it = modules.find(module_name);
			if (it != modules.end())
				return it->second;

			module m(module_name);
			modules[module_name] = m;

			return modules.find(module_name)->second;
		}
	};

}

struct test
{
	std::string name;
	int			value;
};


int _tmain(int argc, _TCHAR* argv[])
{
	//test_bind_stuff();

	meta::storage storage;

	storage.get_module("engine")
		.class_<test>("test")
		.def_readonly("name", &test::name)
		.def("value", &test::value);

	// так же можно и так:
	//storage.get_module("engine").def_class<test>("test");

	//std::string test::*  t = &test::str_value;
	//size_t str_offset = (&test::str_value);//std::cout << ;

	// что еще надо - бинд конструкторов
	// бинд методов
	// бинд св-в через get/set 
	// бинд реад_онли св-в только с get

	
	return 0;
}


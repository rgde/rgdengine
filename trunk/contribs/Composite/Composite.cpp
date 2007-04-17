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

		struct factory
		{ 
			typedef std::map<std::string type_names, type_information*> types_map;
			types_map types;

			void register_type(type_information& ti)
			{
				types[ti.type_name] = &ti;
			}
			

		};

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
			explicit concrete_type_information(const std::string& name)
				: type_information(typeid(T), name)
			{
			}

			template<class BaseType>
			concrete_type_information& base()
			{
				return *this;
			}


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

			template<typename data_type>
			concrete_type_information& def_property(const std::string name, data_type (T::* getter_func_ptr)())
			{
				return *this;
			}

			// нужно использовать boost::type_traits - что бы решить проблему передачи параметров по конст ссылке
			template<typename data_type>
			concrete_type_information& def_property(const std::string name, data_type (T::* getter_func_ptr)(), void (T::* setter_func_ptr)(data_type))
			{
				return *this;
			}

			template<typename data_type>
			concrete_type_information& def(const std::string name, data_type (T::* getter_func_ptr)(), void (T::* setter_func_ptr)(data_type))
			{
				return def_property(name, getter_func_ptr, setter_func_ptr);
			}

			template<typename data_type>
			concrete_type_information& def_property(const std::string name, data_type (T::* getter_func_ptr)() const)
			{
				return *this;
			}

			template<typename data_type>
			concrete_type_information& def_property(const std::string name, data_type (T::* getter_func_ptr)() const, void (T::* setter_func_ptr)(data_type))
			{
				return *this;
			}

			template<typename data_type>
			concrete_type_information& def(const std::string name, data_type (T::* getter_func_ptr)() const, void (T::* setter_func_ptr)(data_type))
			{
				return def_property(name, getter_func_ptr, setter_func_ptr);
			}


			// Mem-func defs
			template<typename ret_type>
			concrete_type_information& def(const std::string name, ret_type (T::* mem_func_ptr)())
			{
				return *this;
			}

			template<typename ret_type, typename P1>
			concrete_type_information& def(const std::string name, ret_type (T::* mem_func_ptr)(P1))
			{
				return *this;
			}

			template<typename ret_type, typename P1, typename P2>
			concrete_type_information& def(const std::string name, ret_type (T::* mem_func_ptr)(P1, P2))
			{
				return *this;
			}

			template<typename ret_type, typename P1, typename P2, typename P3>
			concrete_type_information& def(const std::string name, ret_type (T::* mem_func_ptr)(P1, P2, P3))
			{
				return *this;
			}

			template<typename ret_type, typename P1, typename P2, typename P3, typename P4>
			concrete_type_information& def(const std::string name, ret_type (T::* mem_func_ptr)(P1, P2, P3, P4))
			{
				return *this;
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

struct test_base {};

struct test : public test_base
{	
	int			value;
	std::string name;



	float func0() {return 1.0f;}
	float func1(float f) {return 1.0f*f;}
	float func2(float f, int i) {return 1.0f*f+i;}
	double func3(float f, int i, double d) {return (1.0f*f+i)*d;}

	double get_data() const {return my_private_data;}
	void set_data(double d) {my_private_data = d;}

private:
	double my_private_data;
};


template <class T>
struct mem_func_types_holder
{
	//void (T::* ptfptr) (int) = &X::f;
};

int _tmain(int argc, _TCHAR* argv[])
{
	//test_bind_stuff();

	meta::storage storage;

	storage.get_module("engine")
		.class_<test>("test")
		// можно задать сколько угодно базовых типов. проблема в том что не ясно откуда его получать...
		// видимо тайп инфо должен иметь ссылку на module, а там ссылку на storage...
		.base<test_base>() 
		.def_readonly("name", &test::name)
		.def("value", &test::value)
		.def("func0", &test::func0)
		.def("func3", &test::func3)
		.def_property("data", &test::get_data, &test::set_data)
		.def_property("data_readonly", &test::get_data);

	// так же можно и так:
	//storage.get_module("engine").def_class<test>("test");

	// что еще надо:
	//1
	// бинд конструкторов
	// бинд статик методов

	//2
	// сохранние бид информации. 	
	// наследование

	//3
	// серелизация (bin,xml)
	// удобный доступ из С++

	//4 
	// рефлекшен в скрипт

	//5
	// рефлекшен в редактор

	
	return 0;
}


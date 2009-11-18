#pragma once

namespace meta
{
	namespace details
	{
		template<class T>
		struct concrete_type_information : public type_information
		{
			explicit concrete_type_information(const std::string& name);

			template<class BaseType>
			inline concrete_type_information& base();

			template<typename V>
			inline concrete_type_information& def(const std::string& name, V T::* data_member_pointer);

			template<typename V>
			inline concrete_type_information& def_readonly(const std::string& name, V T::* data_member_pointer);

			template<typename V>
			inline concrete_type_information& def_property(const std::string& name, V T::* data_member_pointer);

			template<typename data_type>
			inline concrete_type_information& def_property(const std::string& name, data_type (T::* getter_func_ptr)());

			// нужно использовать boost::type_traits - что бы решить проблему передачи параметров по конст ссылке
			template<typename data_type>
			inline concrete_type_information& def_property(const std::string& name, data_type (T::* getter_func_ptr)(), void (T::* setter_func_ptr)(data_type));

			template<typename data_type>
			inline concrete_type_information& def(const std::string& name, data_type (T::* getter_func_ptr)(), void (T::* setter_func_ptr)(data_type));

			template<typename data_type>
			inline concrete_type_information& def_property(const std::string& name, data_type (T::* getter_func_ptr)() const);

			template<typename data_type>
			inline concrete_type_information& def_property(const std::string& name, data_type (T::* getter_func_ptr)() const, void (T::* setter_func_ptr)(data_type));

			template<typename data_type>
			inline concrete_type_information& def(const std::string& name, data_type (T::* getter_func_ptr)() const, void (T::* setter_func_ptr)(data_type));

			// Mem-func defs
			template<typename ret_type>
			inline concrete_type_information& def(const std::string& name, ret_type (T::* mem_func_ptr)());

			template<typename ret_type, typename P1>
			inline concrete_type_information& def(const std::string& name, ret_type (T::* mem_func_ptr)(P1));

			template<typename ret_type, typename P1, typename P2>
			inline concrete_type_information& def(const std::string& name, ret_type (T::* mem_func_ptr)(P1, P2));

			template<typename ret_type, typename P1, typename P2, typename P3>
			inline concrete_type_information& def(const std::string& name, ret_type (T::* mem_func_ptr)(P1, P2, P3));

			template<typename ret_type, typename P1, typename P2, typename P3, typename P4>
			inline concrete_type_information& def(const std::string& name, ret_type (T::* mem_func_ptr)(P1, P2, P3, P4));
		};

		//////////////////////////////////////////////////////////////////////////

		template<class T>
		concrete_type_information<T>::concrete_type_information(const std::string& name)
			: type_information(typeid(T), name)
		{
		}

		template<class T>
		template<class BaseType>
		concrete_type_information<T>& concrete_type_information<T>::base()
		{
			return *this;
		}

		template<class T>
		template<typename V>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, V T::* data_member_pointer)
		{
			return *this;
		}

		template<class T>
		template<typename V>
		concrete_type_information<T>& concrete_type_information<T>::def_readonly(const std::string& name, V T::* data_member_pointer)
		{
			return *this;
		}

		template<class T>
		template<typename V>
		concrete_type_information<T>& concrete_type_information<T>::def_property(const std::string& name, V T::* data_member_pointer)
		{
			return def<V>(name, data_member_pointer);
		}

		template<class T>
		template<typename data_type>
		concrete_type_information<T>& concrete_type_information<T>::def_property(const std::string& name, data_type (T::* getter_func_ptr)())
		{
			return *this;
		}

		// нужно использовать boost::type_traits - что бы решить проблему передачи параметров по конст ссылке
		template<class T>
		template<typename data_type>
		concrete_type_information<T>& concrete_type_information<T>::def_property(const std::string& name, data_type (T::* getter_func_ptr)(), void (T::* setter_func_ptr)(data_type))
		{
			return *this;
		}

		template<class T>
		template<typename data_type>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, data_type (T::* getter_func_ptr)(), void (T::* setter_func_ptr)(data_type))
		{
			return def_property(name, getter_func_ptr, setter_func_ptr);
		}

		template<class T>
		template<typename data_type>
		concrete_type_information<T>& concrete_type_information<T>::def_property(const std::string& name, data_type (T::* getter_func_ptr)() const)
		{
			return *this;
		}

		template<class T>
		template<typename data_type>
		concrete_type_information<T>& concrete_type_information<T>::def_property(const std::string& name, data_type (T::* getter_func_ptr)() const, void (T::* setter_func_ptr)(data_type))
		{
			return *this;
		}

		template<class T>
		template<typename data_type>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, data_type (T::* getter_func_ptr)() const, void (T::* setter_func_ptr)(data_type))
		{
			return def_property(name, getter_func_ptr, setter_func_ptr);
		}

		// Mem-func defs
		template<class T>
		template<typename ret_type>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, ret_type (T::* mem_func_ptr)())
		{
			//boost::function_base 
			return *this;
		}

		template<class T>
		template<typename ret_type, typename P1>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, ret_type (T::* mem_func_ptr)(P1))
		{
			return *this;
		}

		template<class T>
		template<typename ret_type, typename P1, typename P2>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, ret_type (T::* mem_func_ptr)(P1, P2))
		{
			return *this;
		}

		template<class T>
		template<typename ret_type, typename P1, typename P2, typename P3>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, ret_type (T::* mem_func_ptr)(P1, P2, P3))
		{
			return *this;
		}

		template<class T>
		template<typename ret_type, typename P1, typename P2, typename P3, typename P4>
		concrete_type_information<T>& concrete_type_information<T>::def(const std::string& name, ret_type (T::* mem_func_ptr)(P1, P2, P3, P4))
		{
			return *this;
		}
	}
}
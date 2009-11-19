#pragma once

#include <boost/any.hpp>

namespace meta
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

		//virtual boost::any get() const = 0;
		//virtual bool set(const boost::any& value) = 0;

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

		//virtual boost::any get() const
		//{
		//	if(can_read())
		//	{

		//	}
		//	return boost::any();
		//}

		//virtual bool set(const boost::any& value) 
		//{

		//}
	};

	template<class OwnerType, class ValueType>
	struct property_instance
	{
		typedef boost::shared_ptr<OwnerType> owner_ptr;
		

	};

	template<class OwnerType, class ValueType>
	property<OwnerType, ValueType>* cast(OwnerType* owner, property_ptr ptr)
	{
		return 0;
	}

	struct property_storage
	{
		typedef std::list<property_ptr> property_list;

		property_list properties;

		template<class OwnerType, class ValueType>
		void register_property(const string& name, 
			typename property<OwnerType, ValueType>::getter_func getter,
			typename property<OwnerType, ValueType>::setter_func setter = property<OwnerType, ValueType>::setter_func())
		{
			properties.push_back(
					property_ptr(
						new property<OwnerType, ValueType>(name, getter, setter)
					)
				);
		}
	};
}
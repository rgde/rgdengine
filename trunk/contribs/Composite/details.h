#pragma once

namespace meta
{
	typedef std::wstring string;
	struct type_information;

	namespace details
	{
		//////////////////////////////////////////////////////////////////////////

		struct factory
		{ 
			typedef std::map<std::string, type_information*> types_map;
			types_map types;

			void register_type(type_information& ti);
		};

		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////

		std::string get_type_name(const type_info& info);
	}
}
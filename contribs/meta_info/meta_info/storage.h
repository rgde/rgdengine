#pragma once

#include "factory.h"

namespace meta
{		
	class types_info_storage
	{
	public:
		types_info_storage()
		{		
			m_factory.set_type_info_storage(this);
		}

		const factory& get_factory() const {return m_factory;}

		template<class T>
		void add(const char* name = 0)
		{
			if (!is_created<T>())
			{
				type_information_ptr ti = create_type_info<T>(name);
				add(ti);
				m_factory.add_type<T>(ti->type_name);
			}
		}

		template<class T, class Base>
		void add(const char* name = 0)
		{
			if (!is_created<T>())
			{
				type_information_ptr ti = create_type_info<T>(name);
				add(ti);
				m_factory.add_type<T>(ti->type_name);

				type_information_ptr& ti_base = find_type<Base>();
				ti->add_parent_type(ti_base);
				//ti_base->add_child_type(ti);
			}
		}

		template<class T>
		type_information_ptr& find_type()
		{
			if (!details::ttype_information<T>::created)
				add<T>(); //TODO: possibly need to inform user if happend

			return types[details::ttype_information<T>::instance->type_name];						
		}

		template<class T>
		type_information_ptr find_type() const
		{
			if (!details::ttype_information<T>::created)
				return type_information_ptr();

			types_map::const_iterator it = types.find(details::ttype_information<T>::instance->type_name);
			return it->second;
		}

		const types_map& get_types() const {return types;}

	protected:
		template<class T>
		bool is_created()
		{
			return details::ttype_information<T>::created;
		}

		template<class T>
		type_information_ptr create_type_info(const char* t_name = 0)
		{
			type_information_ptr t(new type_information(typeid(T), get_type_name<T>(t_name)));
			// adding properties and functions info, potentialy need to use policy
			T::register_class_meta(t->local_properties); 
			details::ttype_information<T>::created = true;
			details::ttype_information<T>::instance = t.get();
			return t;
		}


		void add(const type_information_ptr& ti)
		{
			if (types.find(ti->type_name) == types.end())
				types[ti->type_name] = ti;
		}

		//typedef std::map<const char*>
		struct emty_type{}; // to denoute namespace types [?]

	protected:
		factory m_factory;
		types_map types;
	};
}
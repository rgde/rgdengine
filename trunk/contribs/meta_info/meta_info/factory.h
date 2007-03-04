#pragma once

#include "rtti.h"

namespace meta
{
	struct factory;

	namespace details
	{
		struct abstract_creator
		{
			virtual ~abstract_creator() {}
			// переделать на случай не пустого конструктора. [не очень понятно как]
			// возможно все же есть способ... 
			virtual object_ptr create() const = 0; 
		};

		template<class T>
		struct creator : public abstract_creator
		{
			const factory& owner;
			creator(const factory& f) : owner(f) {}

			virtual object_ptr create() const 
			{
				object_ptr obj(new T());
				obj->get_type_info(owner.get_type_info_storage());
				return obj;
			}
		};

		typedef boost::shared_ptr<abstract_creator> creator_ptr;
	}


	struct factory
	{		
		void set_type_info_storage(types_info_storage* storage) {m_ti_storage = storage;}

		typedef std::map<std::string, details::creator_ptr> creators_map;
		typedef creators_map::const_iterator creators_it;

		creators_map creators;

		const types_info_storage& get_type_info_storage() const {return *m_ti_storage;}

		template<class T>
		void add_type(const std::string& name)
		{
			if (creators.find(name) == creators.end())
			{
				creators[name] = details::creator_ptr(new details::creator<T>(*this));
			}
		}

		object_ptr create(const std::string& name) const
		{
			creators_it it = creators.find(name);

			if (it != creators.end())
			{
				return it->second->create();
			}

			return object_ptr();
		}

	protected:
		types_info_storage* m_ti_storage;
	};
}
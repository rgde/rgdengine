#pragma once

namespace core
{
	template <class T>
	class factory
	{
	public:
		typedef boost::function<T* (void)> creator_func;

	private:
		factory() {}

		typedef std::map <std::string, creator_func>	types_map;
		types_map m_types;

		template<class Derived>
		T* default_creator() {return new Derived();}

	public:
		typedef std::list<std::string>	types_list_t;

		types_list_t types_list() const
		{
			types_list_t	list;
			for (types_map::const_iterator it = m_types.begin(); it != m_types.end(); ++it)
			{
				list.push_back(it->first);
			}

			return list;
		}

		T* create(const std::string& name)
		{
			creator_func creator = m_types[name];
			if (creator)
				return creator();
			else 
				return 0;
		}

		template<class derived_type>
		void register_type(std::string type_name = std::string(), creator_func creator = creator_func())
		{
			creator_func cf = creator ? cf : boost::bind(&factory::default_creator<derived_type>, this);
			if (type_name.empty())
			{
				std::string	temp(typeid(derived_type).name());
				type_name = std::string(temp, 6, temp.size());
			}
			m_types[type_name] = cf;

		}

		static factory<T> &get();
	};


	template <class T>
	factory<T> & factory<T>::get()
	{
		static factory<T> m_factory;
		return m_factory;
	}
}
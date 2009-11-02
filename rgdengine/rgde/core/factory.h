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
		types_map m_types_map;

		template<class Derived>
		T* default_creator() {return new Derived();}

	public:
		typedef std::list<std::string>	types_list;

		types_list get_type_list() const
		{
			types_list	tlist;
			for (types_map::const_iterator it = m_types_map.begin(); it != m_types_map.end(); ++it)
			{
				tlist.push_back(it->first);
			}

			return tlist;
		}

		T* create(const std::string& name)
		{
			creator_func creator = m_types_map[name];
			if (creator)
				return creator();
			else 
				return 0;
		}

		template<class derived_type>
		void register_type(const std::string& type_name = std::string(), creator_func creator = creator_func())
		{
			creator_func cf = creator ? cf : boost::bind(&factory::default_creator<derived_type>, this);
			if (type_name.empty() || type_name == "")
			{
				std::string	temp(typeid(derived_type).name());
				m_types_map[std::string(temp, 6, temp.size())] = cf;
			}
			else
				m_types_map[type_name] = cf;

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
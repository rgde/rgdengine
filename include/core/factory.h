#pragma once

namespace factory
{
	template <class T>
	class TFactory
	{
	public:
		typedef boost::function<T* (void)> CreatorFunc;

	private:
		TFactory() {}

		typedef std::map <std::string, CreatorFunc>	TypeMap;
		TypeMap m_typeMap;

		template<class Derived>
		T* default_creator() {return new Derived();}

	public:
		typedef std::list<std::string>	TypeList;

		TypeList getTypeList() const
		{
			TypeList	tlist;
			for (TypeMap::const_iterator it = m_typeMap.begin(); it != m_typeMap.end(); ++it)
			{
				tlist.push_back(it->first);
			}

			return tlist;
		}

		T* Create(const std::string& name)
		{
			CreatorFunc creator = m_typeMap[name];
			if (creator)
				return creator();
			else 
				return 0;
		}

		template<class Derived>
		void RegisterType(const std::string& type_name = std::string(), CreatorFunc creator = CreatorFunc())
		{
			CreatorFunc cf = creator ? cf : boost::bind(&TFactory::default_creator<Derived>, this);
			if (type_name.empty() || type_name == "")
			{
				std::string	temp(typeid(Derived).name());
				m_typeMap[std::string(temp, 6, temp.size())] = cf;
			}
			else
				m_typeMap[type_name] = cf;

		}

		static TFactory<T> &Instance();
	};


	template <class T>
	TFactory<T> & TFactory<T>::Instance()
	{
		static TFactory<T> m_factory;
		return m_factory;
	}
}
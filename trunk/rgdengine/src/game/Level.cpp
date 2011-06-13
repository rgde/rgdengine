#include "precompiled.h"

#include <rgde/game/level.h>

namespace game
{
	level::level(const std::string& name, const std::string& next_level)
		:	m_name(name),
			m_next_level_name(next_level)
	{
		base::lmsg << "level::level() " << name << base::endl;

		core::function::Func func = boost::bind(&level::addTypeToCreate, this, _1);
		core::functions_owner::add(core::function("AddTypeToCreate", func));
	}

	void level::addTypeToCreate(const std::string& type_name)
	{
		m_listTypes.push_back(type_name);
	}

	level::~level()
	{
		base::lmsg << "level::~level()" << base::endl;
	}

	//инициализация уровня
	void level::enter()
	{
		typedef std::list<std::string> StringList;
		for (StringList::iterator it = m_listTypes.begin(); it != m_listTypes.end(); ++it)
		{
			std::string& type_name = *it;
			level_object* pLevelObj = LevelObjFactory::get().create(type_name);
			m_listLevelObjs.push_back(pLevelObj);
		}
	}

	//деинициализация уровня
	void level::leave()
	{
		//... кажется, что объекты созданные фабрикой надо удалять так:
		//(но могу и соврать)
		std::list<level_object*>::iterator i = m_listLevelObjs.begin();
		while (i != m_listLevelObjs.end())
		{
			delete (*i);
			++i;
		}
		m_listLevelObjs.clear();
	}
}
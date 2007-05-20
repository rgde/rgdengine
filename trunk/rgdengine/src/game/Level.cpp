#include "precompiled.h"

#include "game/Level.h"

namespace game
{
	CLevel::CLevel(const std::string& strName, const std::string& strNextLevel)
		:	m_strName(strName),
			m_strNextLevel(strNextLevel)
	{
		base::lmsg << "CLevel::CLevel() " << strName << base::endl;

		core::Function::Func func = boost::bind(&CLevel::addTypeToCreate, this, _1);
		core::CFunctionsOwner::addFunction(core::Function("AddTypeToCreate", func));
	}

	void CLevel::addTypeToCreate(const std::string& strTypeName)
	{
		m_listTypes.push_back(strTypeName);
	}

	CLevel::~CLevel()
	{
		base::lmsg << "CLevel::~CLevel()" << base::endl;
	}

	//инициализация уровня
	void CLevel::enter()
	{
		typedef std::list<std::string> StringList;
		for (StringList::iterator it = m_listTypes.begin(); it != m_listTypes.end(); ++it)
		{
			std::string& type_name = *it;
			ILevelObject* pLevelObj = LevelObjFactory::Instance().Create(type_name);
			m_listLevelObjs.push_back(pLevelObj);
		}
	}

	//деинициализация уровня
	void CLevel::leave()
	{
		//... кажется, что объекты созданные фабрикой надо удалять так:
		//(но могу и соврать)
		std::list<ILevelObject*>::iterator i = m_listLevelObjs.begin();
		while (i != m_listLevelObjs.end())
		{
			delete (*i);
			++i;
		}
		m_listLevelObjs.clear();
	}
}
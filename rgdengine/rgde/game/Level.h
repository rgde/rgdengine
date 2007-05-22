#pragma once

#include "rgde/core/Property.h"
#include "rgde/game/LevelObject.h"

namespace game
{
	//базовый класс уровня
	class CLevel : public core::CFunctionsOwner
	{
	public:
		CLevel(const std::string& name, const std::string& next_level);
		~CLevel();

		void addTypeToCreate(const std::string& type_name);	//добавление типа для создания
		void enter();											//инициализация уровня
		void leave();											//деинициализация уровня

		//узнать имя уровня
		const std::string& getName() const		{return m_strName;}
		//узнать имя следущего уровня
		const std::string& getNextLevel() const {return m_strNextLevel;}

	private:
		std::list<ILevelObject*> m_listLevelObjs; //список объектов, которые созданы уровнем
		std::list<std::string>   m_listTypes;     //типы объектов которые надо создать
		std::string              m_strName;       //имя уровня
		std::string              m_strNextLevel;  //имя уровня, который должен быть следующим
	};
}
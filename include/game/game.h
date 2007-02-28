#pragma once

#include "event/event.h"

#include "game/Level.h"
#include "game/DynamicObject.h"
#include "game/LevelObject.h"

#include "base/singelton.h"

namespace game
{
	//евент "закончить игру"
	class CCloseGameEvent
	{
	};

	//евент "завершить текущий уровень"
	class CCompliteLevelEvent
	{
	};

	//евент "задать новый уровень"
	class CSetLevelEvent
	{
	public:
		CSetLevelEvent(const std::string& strNextLevel)
			: m_strNextLevel(strNextLevel)
		{
		}

		const std::string& getNextLevel()  const { return m_strNextLevel; }

	private:
		std::string m_strNextLevel;
	};

	//базовый класс игры
	class CGame: public event::CListener
	{
	public:
		void init(const std::string&);
		
		//узнать им€ текущего уровн€
		std::string getCurrentLevel() const {return m_strCurrentLevel;} 

		//задать текущий уровень (по имени)
		void setCurrentLevel(const std::string&);   

		void update();

		void onCloseGame(CCloseGameEvent);
		void onCompliteLevel(CCompliteLevelEvent);
		void onSetLevel(CSetLevelEvent);

		void registerDynamicObject(IDynamicObject*);   //зарегестрировать динамический (т.е. с методом update) объект
		void unregisterDynamicObject(IDynamicObject*); //–ј«регестрировать динамический объект

	private:
		void addLevel(CLevel*);
		CLevel* getLevel(std::string);
	
	protected:
		CGame();
		~CGame();

	private:
		std::string                m_strCurrentLevel;    //им€ текущего уровн€
		std::string                m_strNextLevel;       //им€ следующего уровн€
		std::list<CLevel*>         m_listLevels;         //список уровней
		std::list<IDynamicObject*> m_listDynamicObjects; //список динамических обьектов
		bool                       m_bChangeLevel;
	};

	typedef base::TSingelton<CGame>	TheGame;
}
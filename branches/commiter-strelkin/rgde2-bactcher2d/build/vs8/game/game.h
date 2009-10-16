#pragma once

#include "rgde/event/events.h"

#include "rgde/game/level.h"
#include "rgde/game/dynamic_object.h"
#include "rgde/game/level_object.h"

#include "rgde/base/singelton.h"

namespace game
{
	namespace events
	{
		class on_close_game_event
		{
		};

		class on_complite_level_event
		{
		};

		class on_level_set_event
		{
		public:
			on_level_set_event(const std::string& next_level)
				: m_next_level(next_level)
			{
			}

			const std::string& get_next_level()  const { return m_next_level; }

		private:
			std::string m_next_level;
		};
	}


	class game_system: public event::listener
	{
	public:
        //инициализация из xml файла
		void init(const std::string&); 

        //или динамическое создание уровней игры
        void addLevel(const std::string &name, const std::string &nextlevel);
        void addLevelTypeToCreate(const std::string &name, const std::string& type_name);

		//узнать имя текущего уровня
		const std::string& getCurrentLevel() const {return m_strCurrentLevel;} 

		//задать текущий уровень (по имени)
		void setCurrentLevel(const std::string&);   

		void update();

		void onCloseGame(events::on_close_game_event);
		void onCompliteLevel(events::on_complite_level_event);
		void onSetLevel(events::on_level_set_event);

		void register_object(dynamic_object*);   //зарегестрировать динамический (т.е. с методом update) объект
		void unregister_object(dynamic_object*); //РАЗрегестрировать динамический объект

	private:
		Level* get_level(const std::string& level_name);
	
	protected:
		game_system();
		~game_system();

	private:
		std::string                m_strCurrentLevel;    //имя текущего уровня
		std::string                m_next_level;       //имя следующего уровня
		std::list<Level*>         m_listLevels;         //список уровней
		std::list<dynamic_object*> m_objects; //список динамических обьектов
		bool                       m_change_level;
	};

	typedef base::singelton<game_system>	TheGame;
}
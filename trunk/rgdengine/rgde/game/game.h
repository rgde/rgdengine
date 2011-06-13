#pragma once

#include <rgde/core/Timer.h>

#include <rgde/event/events.h>

#include <rgde/game/level.h>
#include <rgde/game/dynamic_object.h>
#include <rgde/game/level_object.h>

namespace game
{
	namespace events
	{
		class on_close_game
		{
		};

		class on_complite_level
		{
		};

		class on_level_set
		{
		public:
			on_level_set(const std::string& next_level);
			const std::string& next_level()  const { return m_next_level_name; }

		private:
			std::string m_next_level_name;
		};
	}


	class game_system : public event::listener
	{
	public:
		typedef std::list<dynamic_object*> dynamic_objects;
		typedef dynamic_objects::iterator objects_iter;

		game_system();
		~game_system();

        //initialization from XML file
		void init(const std::string&); 

        //or dynamic game levels creation
        void addLevel(const std::string &name, const std::string &next_level);
        void addLevelTypeToCreate(const std::string &name, const std::string& type_name);

		//get name of current game level
		const std::string& getCurrentLevel() const {return m_cur_level_name;} 

		//set current level (by name)
		void setCurrentLevel(const std::string&);   

		void update();

		void onCloseGame(events::on_close_game);
		void onCompliteLevel(events::on_complite_level);
		void onSetLevel(events::on_level_set);

		void register_object(dynamic_object*);   //register dynamic object
		void unregister_object(dynamic_object*); //unregister dynamic object

		const core::timer& get_timer() const {return m_timer;}
		float get_frame_dt() const {return m_cur_frame_delta;}

		static game_system& get();

	private:
		level* get_level(const std::string& level_name);	

	private:
		std::string                m_cur_level_name;	// current level name
		std::string                m_next_level_name;	// next level name
		std::list<level*>          m_levels;			// levels list
		
		dynamic_objects m_objects;			// dynamic objects list

		bool        m_change_level;

		core::timer m_timer;
		float		m_cur_frame_delta;

		static game_system* m_instance;
	};
}
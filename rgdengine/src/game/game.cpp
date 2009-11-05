#include "precompiled.h"

#include <rgde/game/game.h>

#include <rgde/core/timer.h>
#include <rgde/core/application.h>

#include <rgde/render/sprites.h>

namespace game
{
	namespace events
	{
		on_level_set::on_level_set(const std::string& next_level)
			: m_next_level_name(next_level)
		{
		}
	}

	game_system* game_system::m_instance = 0;

	game_system& game_system::get()
	{
		return *m_instance;
	}


	game_system::game_system()
		: m_change_level(false)
	{
		assert(m_instance == 0 && "Error! GameSystem must be only one!");

		m_instance = this;

		subscribe<events::on_close_game>     (&game_system::onCloseGame);
		subscribe<events::on_complite_level> (&game_system::onCompliteLevel);
		subscribe<events::on_level_set>      (&game_system::onSetLevel);
		m_timer.start();
	}

	game_system::~game_system()
	{
		//unsubscribe all dynamic objects
		for (objects_iter it = m_objects.begin();
			it != m_objects.end(); ++it)
		{
			(*it)->unsubscribe();
		}

		m_objects.clear();

		level* current_level = get_level(m_cur_level_name);
		if (0 != current_level)
			current_level->leave();

		while (m_levels.begin() != m_levels.end())
		{
			delete *m_levels.begin();
			m_levels.erase(m_levels.begin());
		}

		m_instance = 0;
	}	


	void game_system::init(const std::string& strXmlGameConfig)
	{
        // skip init if config name are not supplied
        if (strXmlGameConfig == "")
            return;

		std::string fname = "Media/" + strXmlGameConfig;
		TiXmlDocument doc(fname);

		if (!doc.LoadFile())
		{
			//... сообщить, что не удалось загрузить xml файл с конфигурацией программы
			//printf( "Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc() );
		}
		else
		{
			TiXmlNode *root = doc.FirstChild("game");

			if (!root)
				return;

			TiXmlElement *game = root->ToElement();

			if (!game)
				return;

			//узнать начальный уровень игры
			std::string strCurrentLevel = game->Attribute("startlevel");

			//прочитать все уровни, которые относ€тс€ к игре
			TiXmlElement *level_el = game->FirstChildElement("level");

			while (level_el)
			{
				std::string name = level_el->Attribute("name");
				std::string next_level = level_el->Attribute("next_level");

				//добавить уровень
				level* pLevel = new level(name,next_level);
				addLevel(name,next_level);

				//прочитать список объектов, которые должен создать уровень	
				TiXmlElement *levelobject = level_el->FirstChildElement("levelobject");

				while (levelobject)
				{
					std::string type = levelobject->Attribute("type");
					pLevel->call_function("AddTypeToCreate", type);
					levelobject = levelobject->NextSiblingElement("levelobject");
				}

				level_el = level_el->NextSiblingElement("level");
			}

			setCurrentLevel(strCurrentLevel);
		}
	}

    void game_system::addLevel(const std::string &name, const std::string &next_level)
    {
        //убедимс€, что уровней с таким именем нет
        if (get_level(name) != 0)
        {
            //... ругаемс€ в лог и выходим из функции (не добавл€€ нового уровн€)
            return;
        }

        //добавл€ем уровень
		level* pLevel = new level(name,next_level);
		m_levels.push_back(pLevel);
    }

    //написана отдельна€ функци€, лишь бы не давать доступа к
    //укзателю на level: у программиста есть доступ только по имени уровн€
    void game_system::addLevelTypeToCreate(const std::string &name, const std::string& type_name)
    {
        level *pLevel = get_level(name);

        //если такого уровн€ нет
        if (!pLevel)
        {
            //... ругаемс€ в лог и выходим из функции
            return;
        }

        pLevel->call_function("AddTypeToCreate", type_name);
    }


	void game_system::onCloseGame(events::on_close_game)
	{
		setCurrentLevel("");
		core::application::get()->close();
	}

	void game_system::onCompliteLevel(events::on_complite_level)
	{
		std::string next_level;
		level *pLevel = get_level(m_cur_level_name);

		if (0 != pLevel)
			next_level = pLevel->get_next_level();

		setCurrentLevel(next_level);
	}

	void game_system::onSetLevel(events::on_level_set event)
	{
		setCurrentLevel(event.get_next_level());
	}

	void game_system::setCurrentLevel(const std::string& next_level)
	{
		m_change_level = true;
		m_next_level_name = next_level;
	}

	void game_system::update()
	{
		float dt = m_timer.get_elapsed();

		m_cur_frame_delta = dt;

		//static_cast<float>(m_timer.elapsed());
		typedef std::list<dynamic_object*> DinamicObjects;
		typedef DinamicObjects::iterator DinamicObjsIter;

		//проапдейтим все динамические обьекты
		for (DinamicObjsIter it = m_objects.begin();
			it != m_objects.end(); ++it)
		{
			(*it)->update(dt);
		}

		render::TheSpriteManager::get().update();

		//сменим уровень (если надо)
		if (m_change_level)
		{
			level* current_level = get_level(m_cur_level_name);

			if (0 != current_level)
				current_level->leave();

			m_cur_level_name = m_next_level_name;

			current_level = get_level(m_cur_level_name);

			if (0 != current_level)
				current_level->enter();

			m_change_level = false;
		}
	}

	//зарегестрировать динамический (т.е. с методом update) объект
	void game_system::register_object(dynamic_object *obj)
	{
#ifdef _DEBUG		
		std::list<dynamic_object*>::iterator i = find
			(
				m_objects.begin(),
				m_objects.end(),
				obj
			);

		//страховка от двойной регистрации		//(типа, страдаем параноей :))
		if (i != m_objects.end())
			throw std::exception("List of DynamicObjects in game_system corrupted!");

#endif
		m_objects.push_back(obj);
	}

	//–ј«регестрировать динамический объект	
	void game_system::unregister_object(dynamic_object *obj)
	{
#ifdef _DEBUG
		std::list<dynamic_object*>::iterator i = find
			(
				m_objects.begin(),
				m_objects.end(),	
				obj
			);

		if (i == m_objects.end())
			throw std::exception("List of DynamicObjects in game_system corrupted!");
#endif
		m_objects.remove(obj);
	}

	level* game_system::get_level(const std::string& name)
	{
		for(std::list<level*>::iterator i = m_levels.begin(); i != m_levels.end(); ++i)
		{
			if ((*i)->get_name() == name)
				return (*i);
		}
		return 0;
	}
} //namespace game
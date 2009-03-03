#include "precompiled.h"

#include <rgde/game/game.h>

#include <rgde/core/Timer.h>
#include <rgde/core/application.h>

namespace game
{
	//инициализаци€
	void game_system::init(const std::string& strXmlGameConfig)
	{
        //ничего не делаем, если им€ конфига не задано
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
			TiXmlElement *level = game->FirstChildElement("level");

			while (level)
			{
				std::string name = level->Attribute("name");
				std::string nextlevel = level->Attribute("nextlevel");

				//добавить уровень
				Level* pLevel = new Level(name,nextlevel);
				addLevel(name,nextlevel);

				//прочитать список объектов, которые должен создать уровень	
				TiXmlElement *levelobject = level->FirstChildElement("levelobject");

				while (levelobject)
				{
					std::string type = levelobject->Attribute("type");
					pLevel->callFunction("AddTypeToCreate", type);
					levelobject = levelobject->NextSiblingElement("levelobject");
				}

				level = level->NextSiblingElement("level");
			}

			setCurrentLevel(strCurrentLevel);
		}
	}

    void game_system::addLevel(const std::string &name, const std::string &nextlevel)
    {
        //убедимс€, что уровней с таким именем нет
        if (get_level(name) != 0)
        {
            //... ругаемс€ в лог и выходим из функции (не добавл€€ нового уровн€)
            return;
        }

        //добавл€ем уровень
		Level* pLevel = new Level(name,nextlevel);
		m_listLevels.push_back(pLevel);
    }

    //написана отдельна€ функци€, лишь бы не давать доступа к
    //укзателю на Level: у программиста есть доступ только по имени уровн€
    void game_system::addLevelTypeToCreate(const std::string &name, const std::string& type_name)
    {
        Level *pLevel = get_level(name);

        //если такого уровн€ нет
        if (!pLevel)
        {
            //... ругаемс€ в лог и выходим из функции
            return;
        }

        pLevel->callFunction("AddTypeToCreate", type_name);
    }

	game_system::game_system(): m_change_level(false)
	{
		subscribe<events::on_close_game>     (&game_system::onCloseGame);
		subscribe<events::on_complite_level> (&game_system::onCompliteLevel);
		subscribe<events::on_level_set>      (&game_system::onSetLevel);
		core::TheTimer::get().start();
	}

	game_system::~game_system()
	{
		typedef std::list<dynamic_object*> DinamicObjects;
		typedef DinamicObjects::iterator DinamicObjsIter;

		//отписать все динамические объекты
		for (DinamicObjsIter it = m_objects.begin();
			it != m_objects.end(); ++it)
		{
			(*it)->unsubscribe();
		}

		m_objects.clear();

		Level* current_level = get_level(m_strCurrentLevel);
		if (0 != current_level)
			current_level->leave();

        while (m_listLevels.begin() != m_listLevels.end())
        {
            delete *m_listLevels.begin();
            m_listLevels.erase(m_listLevels.begin());
        }
	}	
	
	void game_system::onCloseGame(events::on_close_game)
	{
		setCurrentLevel("");
		core::application::get()->close();
	}

	void game_system::onCompliteLevel(events::on_complite_level)
	{
		std::string next_level;
		Level *pLevel = get_level(m_strCurrentLevel);

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
		m_next_level = next_level;
	}

	void game_system::update()
	{
		float dt = core::TheTimer::get().getElapsedTime();

		//static_cast<float>(m_timer.elapsed());
		typedef std::list<dynamic_object*> DinamicObjects;
		typedef DinamicObjects::iterator DinamicObjsIter;

		//проапдейтим все динамические обьекты
		for (DinamicObjsIter it = m_objects.begin();
			it != m_objects.end(); ++it)
		{
			(*it)->update(dt);
		}

		//сменим уровень (если надо)
		if (m_change_level)
		{
			Level* current_level = get_level(m_strCurrentLevel);

			if (0 != current_level)
				current_level->leave();

			m_strCurrentLevel = m_next_level;

			current_level = get_level(m_strCurrentLevel);

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

	Level* game_system::get_level(const std::string& name)
	{
		for(std::list<Level*>::iterator i = m_listLevels.begin(); i != m_listLevels.end(); ++i)
		{
			if ((*i)->getName() == name)
				return (*i);
		}
		return 0;
	}
} //namespace game

#include "MapLevelObject.h"
#include <iostream>

MapLevelObject::MapLevelObject()
{
    std::cout << "3: MapLevelObject created. (press SPACE for send events::on_complite_level)" << std::endl;

    {
        using namespace input;

        m_cSpace.attach(L"Space");
		Input::getDevice(types::Keyboard)->get_control(types::KeySpace )->bind(L"Space");        
        m_cSpace += boost::bind(&MapLevelObject::onSpace, this);
    }
}

MapLevelObject::~MapLevelObject()
{
	std::cout << "MapLevelObject destroyed." << std::endl;
}

void MapLevelObject::onSpace()
{
	send_event<game::events::on_complite_level>(game::events::on_complite_level());
}

namespace game
{
  REGISTER_LEVEL_TYPE(MapLevelObject)
}
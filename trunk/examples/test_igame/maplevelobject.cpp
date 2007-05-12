
#include "MapLevelObject.h"
#include <iostream>

MapLevelObject::MapLevelObject()
{
    std::cout << "3: MapLevelObject created. (press SPACE for send CCompliteLevelEvent)" << std::endl;

    {
        using namespace input;

        m_cSpace.attach(L"Space");
		Input::getDevice(types::Keyboard)->getControl(types::KeySpace )->bind(L"Space");        
        m_cSpace += boost::bind(&MapLevelObject::onSpace, this);
    }
}

MapLevelObject::~MapLevelObject()
{
	std::cout << "MapLevelObject destroyed." << std::endl;
}

void MapLevelObject::onSpace()
{
	sendEvent<game::CCompliteLevelEvent>(game::CCompliteLevelEvent());
}

namespace game
{
  REGISTER_LEVEL_TYPE(MapLevelObject)
}
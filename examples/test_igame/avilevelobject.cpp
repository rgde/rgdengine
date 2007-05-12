//AviLevelObject.cpp
#include "AviLevelObject.h"
#include <iostream>

AviLevelObject::AviLevelObject()
{
	std::cout << "1: AviLevelObject created. (press SPACE for send CCompliteLevelEvent)\n";

    {
        using namespace input;

        m_cSpace.attach(L"Space");
		Input::getDevice(types::Keyboard)->getControl(types::KeySpace )->bind(L"Space");        
        m_cSpace += boost::bind(&AviLevelObject::onSpace, this);
    }
}

AviLevelObject::~AviLevelObject()
{
	std::cout << "AviLevelObject destroyed." << std::endl;
}

void AviLevelObject::onSpace() 
{
	sendEvent<game::CCompliteLevelEvent>(game::CCompliteLevelEvent());
}

namespace game
{
	REGISTER_LEVEL_TYPE(AviLevelObject)
}
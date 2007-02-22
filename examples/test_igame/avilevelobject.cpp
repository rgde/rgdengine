//AviLevelObject.cpp
#include "AviLevelObject.h"
#include <iostream>

AviLevelObject::AviLevelObject()
{
	std::cout << "1: AviLevelObject created. (press SPACE for send CCompliteLevelEvent)\n";

    {
        using namespace input;

        CInput::addCommand(L"Space");
        CInput::getDevice(Keyboard)->getControl(KeySpace )->bind(L"Space");
        m_cSpace.attach(L"Space");
        m_cSpace += boost::bind(&AviLevelObject::onSpace, this);
    }
}

AviLevelObject::~AviLevelObject()
{
	std::cout << "AviLevelObject destroyed." << std::endl;
}

void AviLevelObject::onSpace() 
{
	sendEvent(game::CCompliteLevelEvent());
}

namespace game
{
	REGISTER_LEVEL_TYPE(AviLevelObject)
}
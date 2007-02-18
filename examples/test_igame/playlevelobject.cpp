//PlayLevelObject.cpp
#include "PlayLevelObject.h"
#include <iostream>

PlayLevelObject::PlayLevelObject()
: m_glass(math::Rect(50, 50, 320, 480), 20)
{
	std::cout << "3: PlayLevelObject created." << std::endl;
}

PlayLevelObject::~PlayLevelObject()
{
	std::cout << "PlayLevelObject destroyed." << std::endl;
}

namespace game
{
	REGISTER_LEVEL_TYPE(PlayLevelObject)
}
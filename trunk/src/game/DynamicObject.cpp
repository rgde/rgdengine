#include "precompiled.h"

#include "game/DynamicObject.h"

#include "game/game.h"

namespace game
{
	IDynamicObject::IDynamicObject()
	{
		game::TheGame::Get().registerDynamicObject(this);
		m_bIsSubscribed = true;
	}

	IDynamicObject::~IDynamicObject()
	{
		if (m_bIsSubscribed)
			game::TheGame::Get().unregisterDynamicObject(this);
	}
}
#include "precompiled.h"

#include <rgde/game/dynamic_object.h>
#include <rgde/game/game.h>

namespace game
{
	dynamic_object::dynamic_object()
	{
		game::TheGame::Get().register_object(this);
		m_is_subscribed = true;
	}

	dynamic_object::~dynamic_object()
	{
		if (m_is_subscribed)
			game::TheGame::Get().unregister_object(this);
	}
}
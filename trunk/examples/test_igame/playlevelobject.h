#pragma once
#include <rgde/engine.h>
#include "glass.h"

//игра
class PlayLevelObject : public game::level_object, public event::sender
{
public:
	PlayLevelObject();
	~PlayLevelObject();

private:
	game::Glass m_glass;
};
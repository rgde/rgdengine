#pragma once
#include <rgde/engine.h>

//карта
class MapLevelObject : public game::level_object, public event::sender
{	
public:
    MapLevelObject();
   ~MapLevelObject();

private:
	void onSpace();

private:
	input::KeyDown m_cSpace;
};
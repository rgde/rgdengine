#pragma once
#include <rgde/engine.h>

//карта
class MapLevelObject : public game::ILevelObject, public event::CSender
{	
public:
    MapLevelObject();
   ~MapLevelObject();

private:
	void onSpace();

private:
	input::KeyDown m_cSpace;
};
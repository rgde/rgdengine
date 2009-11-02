#pragma once
#include <rgde/engine.h>

//проигрывание роликов
class AviLevelObject: public game::level_object, public event::sender
{
public:
    AviLevelObject();
   ~AviLevelObject();

private:
	void onSpace();

private:
	input::key_down m_cSpace;
};
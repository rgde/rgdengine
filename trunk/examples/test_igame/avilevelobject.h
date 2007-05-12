#pragma once
#include "engine.h"

//проигрывание роликов
class AviLevelObject: public game::ILevelObject, public event::CSender
{
public:
    AviLevelObject();
   ~AviLevelObject();

private:
	void onSpace();

private:
	input::KeyDown m_cSpace;
};
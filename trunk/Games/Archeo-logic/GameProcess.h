#pragma once
#include <engine.h>

#include "GameField.h"

class GameProcess : public game::ILevelObject, 
					public event::CSender,
					public game::IDynamicObject
{
public:
	GameProcess(void);
	~GameProcess(void);

	void update(float dt);

private:

	GameField m_field;

};


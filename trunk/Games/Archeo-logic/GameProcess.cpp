#include "GameProcess.h"

GameProcess::GameProcess(void)
{
//	io::ScopePathAdd p("dagger/");
//	m_field.LoadLevelFromFile("Image2.dag");
}

GameProcess::~GameProcess(void)
{
}


void GameProcess::update(float dt)
{
	m_field.Draw();
}


namespace game
{
	REGISTER_LEVEL_TYPE(GameProcess)
}

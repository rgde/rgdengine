#include <engine.h>

using namespace core;
using namespace game;


int APIENTRY WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::auto_ptr<core::IApplication> pApp(core::IApplication::Create(L"RGDE: DaGGeR's Game", 800, 600, false,false));
	pApp->addTask(PTask(new RenderTask(*pApp, 0)));
	pApp->addTask(PTask(new InputTask(*pApp, 1, false)));
	pApp->addTask(PTask(new CGameTask(*pApp, 2)));
	game::TheGame::Get().addLevel("game","game");
	game::TheGame::Get().addLevelTypeToCreate("game", "GameProcess");
	game::TheGame::Get().setCurrentLevel("game");
	pApp->Run();

	return 0;
}
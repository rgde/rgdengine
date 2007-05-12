#include "sprites.h"

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::auto_ptr<core::IApplication> pApp(core::IApplication::Create(L"Test_Sprites Example", 640, 480, false));
	pApp->addTask(core::PTask(new core::InputTask(*pApp, 0, false)));
	pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
	pApp->addTask(core::PTask(new core::RenderTask(*pApp, 2)));

	CSpriteTest test;

	pApp->Run();	
}
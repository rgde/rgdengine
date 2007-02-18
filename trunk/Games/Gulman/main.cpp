#include "game.h"



int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::auto_ptr<core::IApplication> pApp(core::IApplication::Create());

	pApp->addTask(core::PTask(new core::CInputTask(*pApp, 0, false)));
	pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
	pApp->addTask(core::PTask(new core::CRenderTask(*pApp, 2)));
	
	gulman::Game g;

	pApp->Run();

	return 0;
}
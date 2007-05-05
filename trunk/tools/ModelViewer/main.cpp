#include "ModelViewer.h"

int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	typedef std::auto_ptr<core::IApplication> PApp;
	PApp pApp(core::IApplication::Create(L"Model Viewer", 800, 600, false, true));

	{
		typedef core::PTask PTask;
		PTask input_task(new core::CInputTask(*pApp, 0, false));
		PTask game_task(new core::CGameTask(*pApp, 1));
		PTask render_task(new core::CRenderTask(*pApp, 2));

		{
			pApp->addTask(input_task);
			pApp->addTask(game_task);
			pApp->addTask(render_task);

			ModelViewer r;

			pApp->Run();
		}
	}

	return 0;
}
#include <engine.h>
#include "conio.h"

#include "SnowEffect.h"

using namespace core;
using namespace game;

typedef std::auto_ptr<IApplication> PApplication;

int main()
{
	{
		PApplication pApp(IApplication::Create(L"RGDE: Game Test", 640, 480, false));

		pApp->addTask(PTask(new CRenderTask(*pApp, 0)));
		pApp->addTask(PTask(new CInputTask(*pApp, 1, false)));
		pApp->addTask(PTask(new CGameTask(*pApp, 2, "gameconfig.xml")));

		game::effects::SnowEffect background_effect;

		pApp->Run();
	}

	//_getch();

	return 0;
}
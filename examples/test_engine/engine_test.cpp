#include <engine.h>

#include <boost/scoped_ptr.hpp>

void main()
{
	typedef boost::scoped_ptr<core::IApplication> PApplication;
	PApplication pApp(core::IApplication::Create(L"Engine test", 800, 600, false));

	pApp->addTask(core::PTask(new core::CInputTask(*pApp, 0, false)));
	pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
	pApp->addTask(core::PTask(new core::CRenderTask(*pApp, 2)));
	
	pApp->Run();
}

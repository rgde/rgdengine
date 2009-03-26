#include "stdafx.h"

//#include <d3dx9.h>

//using namespace rgde;
//using core::windows::window;
//using render::device;
//using render::font;
//using namespace math;

#include "Application.h"


//int _tmain(int argc, wchar_t* argv[])
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	rgde::Application app;

	app.run();
	
	return 0;
}


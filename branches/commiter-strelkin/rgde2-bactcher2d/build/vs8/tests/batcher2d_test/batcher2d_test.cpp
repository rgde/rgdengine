#include "stdafx.h"
#include "BatcherApplication.h"

int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::string title("Batcher2D test");
	std::wstring wtitle( title.begin(), title.end() );
	rgde::BatcherApplication app(250, 50, 640, 480, wtitle);
	app.run();

	return 0;
}


#include "stdafx.h"
#include "BatcherApplication.h"

int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	rgde::BatcherApplication app(250, 50, 640, 480, L"Batcher2D test");

	app.run();

	return 0;
}


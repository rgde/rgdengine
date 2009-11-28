// game_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameApplication.h"

int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	rgde::GameApplication app(250, 50, 640, 480, L"Batcher2D test");

	app.run();

	return 0;
}


// game_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameApplication.h"

int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	wchar_t buf[512];
	GetModuleFileNameW(NULL, &buf[0], 512);

	boost::filesystem::wpath p(buf);
	std::wstring path = p.branch_path().string() + L"/../data/";
	SetCurrentDirectoryW(path.c_str());

	rgde::GameApplication app(250, 50, 800, 600, L"Batcher2D test");

	app.run();

	return 0;
}


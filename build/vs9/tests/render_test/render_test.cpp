#include "stdafx.h"

#include "application.h"


int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	wchar_t buf[512];
	GetModuleFileNameW(NULL, &buf[0], 512);

	boost::filesystem::wpath p(buf);
	std::wstring path = p.branch_path().string() + L"/../data/";
	SetCurrentDirectoryW(path.c_str());

	rgde::application app(100, 100, 800, 600, L"Render Test");

	app.run();
	
	return 0;
}


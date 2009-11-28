#include "stdafx.h"
#include "BatcherApplication.h"

int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	wchar_t buf[512];
	GetModuleFileNameW(NULL, &buf[0], 512);

	boost::filesystem::wpath p(buf);
	std::wstring path = p.branch_path().string() + L"/../data/";
	SetCurrentDirectoryW(path.c_str());

	rgde::BatcherApplication app(250, 50, 640, 480, L"Batcher2D test");

	app.run();

	return 0;
}


#include "stdafx.h"

#include "Application.h"


int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	wchar_t buf[512];
	GetModuleFileNameW(NULL, &buf[0], 512);
	
	boost::filesystem::wpath p(buf);
	std::wstring path = p.branch_path().string() + L"/../data/";
	SetCurrentDirectoryW(path.c_str());

	xml::document config;
	bool res = config.load_file("config.xml");

	xml::node app_node = config("root")("app"); 

	std::wstring title = xml::as_utf16(app_node("title").first_child().value());

	int x, y, w, h;
	sscanf_s(app_node("rect")["value"].value(), "%d %d %d %d", &x, &y, &w, &h);

	{
		rgde::Application app(x, y, w, h, title);
		app.run();
	}
	
	return 0;
}


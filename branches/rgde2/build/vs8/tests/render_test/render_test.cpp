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

	SetCurrentDirectoryW(L"../data/");

	std::wstring buf;
	buf.resize(256);
	GetModuleFileNameW(NULL, &buf[0], 256);

	std::wstring module_path;
	module_path.resize(256);
	GetFullPathNameW(&buf[0], 256, &module_path[0], NULL);

	//using boost::filesystem::path;
	//boost::filesystem::wpath p(buffer);
	//std::wstring path = p.branch_path().string();
	//SetCurrentDirectoryW(path.c_str());


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


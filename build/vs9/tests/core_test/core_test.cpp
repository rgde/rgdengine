#include "stdafx.h"

#include <rgde/core/core.h>
#include <rgde/log/log.h>
#include <rgde/core/options/options.h>




namespace test
{
	struct resource
	{
		resource(const std::wstring& name)
		{
			std::wcout << "resource() " << name.c_str() << std::endl;
		}

		~resource()
		{
			std::wcout << "~resource()" << std::endl;
		}
	};

	typedef boost::shared_ptr<resource> resource_ptr;

	resource_ptr create_resource(const std::wstring& name)
	{
		return resource_ptr(new resource(name));
	}
}


int _tmain(int argc, wchar_t* argv[])
{
	using namespace rgde;
	using namespace std;

	using namespace rgde::log;

	logsystem log; //пока существует экземпл€р класса logsystem лог работает

	//wcout.rdbuf(log.rdbuf());

	lmsg << L"Test" << endl;
	{
		core::timer timer;
		//Sleep(1000);
		float elapsed = timer.get_elapsed_time();
		lmsg << L"elapsed time: " << elapsed << endl;
		//new char[100];
		core::log::utils::debug_out(L"DEBUG LOG TEST MSG");
	}

	core::utils::get_allocated_mem();

	{	// math test
		math::vec3f v1(1, 2, 3);
		math::vec3f v2(2, 1, 0);
		math::vec3f v3 = v1 + v2;
		//cout << v3 << endl;
	}

	{
		core::windows::window window(L"TestWindow");

		window.show();
		window.update();
	}

	try
	{
		//throw core::errors::not_implemented("actor::clone");
		using namespace test;
		typedef core::utils::resouce_manager<resource, std::wstring> resouce_manager;

		resouce_manager manager(boost::bind<resource_ptr>(&create_resource, _1));

		resource_ptr res1 = manager.get(L"file1.dat");
		resource_ptr res2 = manager.get(L"file2.dat");
	}
	//catch(core::errors::not_implemented& ex)
	//{

	//}
	catch(core::errors::core_exception& ex)
	{
		cout << ex.what() << endl;
	}

	// TEST MEM LEAKS REPORTING
	//new int[10];
	
	return 0;
}
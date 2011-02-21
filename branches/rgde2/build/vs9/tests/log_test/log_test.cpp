#include "stdafx.h"

#include <rgde/log/log.h>

#include <windows.h>
#include <boost/filesystem.hpp>

int _tmain(int argc, _TCHAR* argv[])
{	
	wchar_t buf[512];
	GetModuleFileNameW(NULL, &buf[0], 512);

	boost::filesystem::wpath p(buf);
	std::wstring path = p.branch_path().string() + L"/../data/";
	SetCurrentDirectoryW(path.c_str());


    using namespace rgde::log;

    logsystem log; //пока существует экземпляр класса logsystem лог работает

    lmsg << "(lmsg) Сообщение в лог: " << 1 << std::endl;
    lwrn << "(lwrn) Предупреждение в лог: " << 2 << std::endl;
    lerr << "(lerr) Ошибка в лог: " << 3 << std::endl;

    lmsg << L"(wlmsg) Сообщение в лог: " << 4 << std::endl;
    lwrn << L"(wlwrn) Предупреждение в лог: " << 5 << std::endl;
    lerr << L"(wlerr) Ошибка в лог: " << 6 << std::endl;

    std::cout << "(cout) Сообщение в лог: " << 7 << std::endl;
    std::wcout << L"(wcout) Сообщение в лог: " << 8 << std::endl;

    std::cerr << "(cerr) Ошибка в лог: " << 9 << std::endl;
    std::wcerr << L"(wcerr) Ошибка в лог: " << 10 << std::endl;

	return 0;
}


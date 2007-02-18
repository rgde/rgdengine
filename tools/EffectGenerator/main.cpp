
#include "stdafx.h"
#include "Generator.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TheGenerator::Get().readInputFile("Config.xml");
	TheGenerator::Get().generate();

	return 0;
}
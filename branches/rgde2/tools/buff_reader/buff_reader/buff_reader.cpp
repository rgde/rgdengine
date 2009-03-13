// buff_reader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "mesh_reader.h"

const std::string def_test_file = "./ch_arrawn_body38373016.buf";


int _tmain(int argc, _TCHAR* argv[])
{

	mesh_info mesh;

	read_mesh(def_test_file.c_str(), mesh);
	save_mesh("./new_mesh.mesh", mesh);
	

	return 0;
}


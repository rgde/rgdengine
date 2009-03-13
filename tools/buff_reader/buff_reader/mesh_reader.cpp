#include <stdafx.h>
#include "mesh_reader.h"

#include <string>

#include <iostream>
#include <fstream>

namespace 
{
	template<class T>
	void read(std::ifstream& in, T& value)
	{
		in.read((char*)&value, sizeof(T));
	}

	template<class T>
	void read(std::ifstream& in, T* value, size_t num_elements)
	{
		in.read((char*)value, sizeof(T)*num_elements);
	}
}


void save_mesh(const char* file_name, const mesh_info& mesh)
{
	std::ofstream out(file_name, std::ios::binary);

	size_t total_size = sizeof(mesh_info) + mesh.num_bones*sizeof(bone_info)
		+ mesh.vertex_size * mesh.num_vertices 
		+ mesh.index_size * mesh.num_indices
		+ sizeof(int)*mesh.num_mats
		+ sizeof(attribute_range)*mesh.num_mats;

	out.write((const char*)mesh, sizeof(mesh_info));
}

bool read_mesh(const char* file_name, mesh_info& mesh)
{
	std::ifstream in(file_name, std::ios::binary);

	using std::cout;
	using std::endl;

	cout << "file: " << file_name << endl;


	if (in.is_open())
	{
		read(in, mesh.version);
		cout << "version: " << (int)mesh.version << endl;

		int lod_num;
		read(in, lod_num);
		cout << "lod_num: " << (int)lod_num << endl;
		assert(lod_num == 1 && "Invalid lod num!");

		read(in, mesh.num_bones);
		cout << "num_bones: " << (int)mesh.num_bones << endl;

		mesh.bones = new bone_info[mesh.num_bones];

		for(int i=0;i<mesh.num_bones;i++)
		{
			bone_info& bone = mesh.bones[i];

			int name_size;
			read(in, name_size);
			bone.name = new char[name_size+1];
			bone.name[name_size] = 0;
			in.read(bone.name, name_size);

			read(in, bone.offset);

			read(in, bone.num_indices);
			bone.indices = new dword[bone.num_indices];
			read(in, bone.indices, bone.num_indices);

			read(in, bone.num_weights);
			bone.weights = new float[bone.num_weights];
			read(in, bone.weights, bone.num_weights);

			cout << "bone name: " << bone.name << endl;
			cout << "num weights: " << bone.num_weights << " weights: " << endl;

			for (unsigned int bi =0;  bi < bone.num_weights; ++bi)
			{
				cout << bone.weights[bi] << ";";
			}
			cout << endl;
		}

		bool optimized;
		read(in, optimized);

		int num_segments;
		read(in, num_segments);

		read(in, mesh.vertex_size);
		read(in, mesh.num_vertices);

		mesh.vb = new char[mesh.num_vertices*mesh.vertex_size];
		read(in, mesh.vb, mesh.num_vertices*mesh.vertex_size);

		mesh.vertex_type = size_to_type(mesh.vertex_size);

		read(in, mesh.index_size);
		read(in, mesh.num_indices);

		mesh.ib = new char[mesh.num_indices*mesh.index_size];
		read(in, mesh.ib, mesh.num_indices*mesh.index_size);

		read(in, mesh.num_mats);

		mesh.mat_ids = new int[mesh.num_mats];
		read(in, mesh.mat_ids, mesh.num_mats);

		assert(mesh.num_mats == num_segments && "invalid num segments!");

		mesh.atribs = new attribute_range[mesh.num_mats];

		for(int i = 0; i < mesh.num_mats; ++i)
		{
			read(in, mesh.atribs[i]);

			int num_atrib_remap;
			read(in, num_atrib_remap);

			assert(num_atrib_remap == 0 && "Invalid mesh format!");
		}

		__asm nop;
	}
	else
	{
		__asm nop;
	}

	return true;
}

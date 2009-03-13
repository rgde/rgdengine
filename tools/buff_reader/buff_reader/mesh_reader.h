#pragma once

#include "vert_types.h"

struct bone_info
{	
	dword num_indices;
	dword* indices;
	dword num_weights;
	float* weights;
	char* name;
	float offset[16];
};

struct attribute_range
{
	dword attrib_id;
	dword face_start;
	dword face_count;
	dword vertex_start;
	dword vertex_count;
};

struct mesh_info
{
	byte version;

	int num_bones;
	bone_info* bones;

	int vertex_size;
	int num_vertices;
	char* vb;

	int index_size;
	int num_indices;
	char* ib;

	int num_mats;
	int* mat_ids;
	attribute_range* atribs;

	vertex_types vertex_type;
};

bool read_mesh(const char* file_name, mesh_info& mesh);
void save_mesh(const char* file_name, const mesh_info& mesh);
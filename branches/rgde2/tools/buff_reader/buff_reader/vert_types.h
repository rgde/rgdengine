#pragma once

#include "types.h"

enum vertex_types
{
	standart = 0,
	standart_t2,
	vertex_color,
	color_t2,
	sh_vrtx,
	packed,
	skinned,
	skinned_t2,
	simple,
	st_grass,
	terrain,
	tex_skinned
};

namespace details
{
	template<typename T, unsigned int size>
	struct data_holder
	{
		T data[size];
	};
}

typedef details::data_holder<short, 2> short2;
typedef details::data_holder<short, 4> short4;
typedef details::data_holder<unsigned short, 4> ushort2;
typedef details::data_holder<ubyte, 4> ubyte4;

typedef details::data_holder<float, 2> vector2;
typedef details::data_holder<float, 3> vector3;
typedef details::data_holder<float, 4> vector4;

typedef dword color;

#pragma pack( push, 1 )


//--------------------------------------------------------------------------------------
/// Vertex structure
//--------------------------------------------------------------------------------------
struct Vertex 
{
	vector3	position; 
	vector3	normal;
	vector3	tan;
	vector3	binormal;
	vector2	tex;
};

//--------------------------------------------------------------------------------------
/// Vertex with compressed data
//--------------------------------------------------------------------------------------
struct PackedVertex 
{
	vector3  pos; 
	short4  norm;
	short4  tan; 
	short4  binormal; 
	short2  tex;
};

//--------------------------------------------------------------------------------------
/// ColVertex structure
//--------------------------------------------------------------------------------------
struct ColVertex 
{
	vector3	position; 
	vector3	normal;
	vector3	tan;
	vector3	binormal;
	vector2	tex;
	dword	col;
};

//--------------------------------------------------------------------------------------
/// Vertex structure with 2 texcoords
//--------------------------------------------------------------------------------------
struct VertexT2
{
	vector3	position; 
	vector3	normal;
	vector3	tan;
	vector3	binormal;
	vector2	tex[2];
};

//--------------------------------------------------------------------------------------
/// Vertex structure with 2 texcoords & vertex color
//--------------------------------------------------------------------------------------
struct ColVertexT2
{
	vector3	position; 
	vector3	normal;
	vector3	tan;
	vector3	binormal;
	vector2	tex[2];
	dword	col;
};


struct SHVertex 
{
	vector3	position; 
	vector3	normal;
	vector3	tan;
	vector3	binormal;
	float	tu, tv;
	int		iClusterOffset;
	vector4	vPCAWeights[24/4];
};

//--------------------------------------------------------------------------------------
/// Vertex used on skinned meshes
//--------------------------------------------------------------------------------------
struct SkinnedVertex 
{
	vector3	position;
	float	blend1,blend2,blend3;
	dword	indices;
	vector3	normal;
	float	tu,tv;
	vector3	tan;
	vector3	binormal;
};

struct LVertex 
{
	vector3	position;
	ulong	diffuse;
	float   tu, tv;
};


struct TLVertex 
{
	vector4 position;
	ulong	diffuse;
	float   tu, tv;
};

struct SimpleVertex
{
	vector3 position;
};

struct STGrassVertex
{
	vector3		position;
	vector3		normal;
	float		TexCoords[2];
	float		BillBoardShifts[2];
	float		WindAttribs[2];
	color		col;
	color		aperture;
};

#pragma pack( pop )


inline vertex_types size_to_type(size_t size)
{
	switch(size)
	{
	case sizeof(Vertex):
		return standart;
	case sizeof(VertexT2):
		return standart_t2;
	case sizeof(ColVertex):
		return vertex_color;
	case sizeof(ColVertexT2):
		return color_t2;
	case sizeof(PackedVertex):
		return packed;
	case sizeof(SkinnedVertex):
		return skinned;
	case sizeof(SHVertex):
		return sh_vrtx;
	case sizeof(vector3):
		return simple;
	default: // "Vertex format of mesh is not supported"
		return (vertex_types)-1;
	}
}
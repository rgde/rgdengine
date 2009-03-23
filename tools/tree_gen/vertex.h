#pragma once


#define VERTEX_FORMAT D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_TEX1

class vertex
{
public:
	float x, y, z;	//coords
	float nx,ny,nz;	//normal
	float tu, tv;	//texture coords

	vertex() {}
	vertex(float _x,float _y,float _z) 
	{
		ZeroMemory(this,sizeof(vertex)); 
		x=_x;y=_y;z=_z; ny=1;
	}

	vertex(const vec3& v) 
	{ 
		ZeroMemory(this,sizeof(vertex)); 
		x=v.x; y=v.y; z=v.z; ny=1; 
	}

	vertex(const vec3& v, const vec3& n, float _tu, float _tv) 
	{
		x=v.x; y=v.y; z=v.z;
		nx=n.x; ny=n.y; nz=n.z;
		tu=_tu; tv=_tv;
	}

	inline void operator=(const vec3& b){ x=b.x; y=b.y; z=b.z; }
	inline vec3 pos(){return vec3(x,y,z); }
	inline vec3 norm(){return vec3(nx,ny,nz); }
	inline void set_norm(const vec3& n){ nx=n.x; ny=n.y; nz=n.z; }
};
#pragma once

struct branch_params
{
	vec3 base;	//start position of branch
	vec3 dir;	//start direction of grow
	float baser;	//start branch radius
	float topr;		//end branch radius
	float lenght;	//total lenght of branch
	float anglel;	//min branch twist per step
	float angleh;	//max branch twist per step
	float posrnd;	//grow direction randomize
	float radrnd;	//branch radius randomize
	float folimin;	//min foliages per step
	float folimax;	//max foliages per step
	float folisizemin;	//min size of foliage
	float folisizemax;	//max size of foliage
	float folimassmin;	//min mass of foliage (will pull it to grownd)
	float folimassmax;	//max mass of foliage (will pull it to grownd)
	float fatroot;	//first section radius multiplier - for root
	float branch;	//branching factor (1 will generate new branch every step,0-no branches)
	float maxthickf;//max radius of branch that foliage grow from
	int sections;	//total sections of branch
	int sectors;	//prism planes at begin of branch
	branch_params()
	{
		ZeroMemory( this, sizeof(branch_params));
	}
};

class tree
{
public:
}

class tree_builder
{
public:
	tree* build();

private:
	DWORD SectionTriangles( DWORD index, WORD n1, WORD n2, DWORD *out );
	void AddFoliage( const vec3& pos, const vec3& dir, float size, float weight, vertex *v, WORD *t, WORD index );
	void CreateBranch( branch_params &b );
	void AddBranchSection(	int n, const vec3& center, const vec3& normal, float radius, float angle,
								vertex *vpt, float tv, float radrnd);

private:
	rand_table rnd;

	std::vector<vertex> v(65000);
	DWORD vertices = 0;

	std::vector<DWORD> t(400000);
	DWORD indices = 0;

	//foliage mesh
	std::vector<vertex> vfl(65000);
	DWORD verticesf;
	std::vector<WORD> tfl(100000);
	DWORD indicesf;

	//int const STMAX=32;
	std::vector<branch_params> stack(32);
	int top=0;
}
#pragma once

namespace rgde {
namespace render {

enum value_type {
	float_t, 
	int_t, 
	bool_t, 
	vec3_t,
	vec4_t,
	color_t, 
	texture_t, 
	mat44f_t
};

//Exposed render params (binding to shaders)
enum exp_param_type {
	modelview_p,
	modelviewproj_p;
	world_p,
	amblight_p
};

struct render_param_desc {
	const char* name;
	exp_param_type type;	
	value_type val_type;
};

// standard engine params (or not needed?)
render_param_desc params_desc[] = {
	{"MODELVIEW", modelview_p, mat44f_t},
	{"AMBCOLOR", amblight_p, color_t}
};

// base(common) shader class
struct shader {
	const char* filename;
	const char* compilation_flags;
};

// vertex shader
struct vshader : public shader {
};

//fragment shader
struct fshader : public shader {
};

//geometry
struct gshader : public shader {
};

struct technique {
	const char* name;
	// may (and should) be used as meta-information storage
	const char* user_desc;
	vshader* vs;
	fshader* fs;
	gshader* gs;
};

struct gpu_programm {
	// first tech - is default
	technique* techniques;
	size_t num_techniques;

	technique* find_tech(const char* name);	
};

struct param_value {
	union{
		float pf;
		int pi;
		bool pb;
		math::vec3f pv3;
		math::vec4f pv4;
		math::color pcolor;
		texture_ptr ptexture;
	};
	float amount;
};

struct mat_param {
	value_type type;
	const char* param_name;
	const char* param_group;

	param_value base_value;
};


struct param_instace
{
	mat_param* param_desc;
	param_value* value;
};

struct material_desc {
	typedef std::vector<mat_param> params_t;
	params_t params;
	const char* name;
};

struct material {
	typedef std::vector<param_instace> params_t;

	material_ptr clone();

	material_desc* desc;	
	params_t params;
	gpu_programm* prog;
};

class material_manager {
public:
	typedef int handle;

	material_ptr create(const char* name);
	material_ptr load(const char* filename);

	handle bind(const char* name, int& value);
	handle bind(const char* name, float& value);
	handle bind(const char* name, math::color& value);
	handle bind(const char* name, math::vec3& value);
	handle bind(const char* name, math::vec4& value);
	handle bind(const char* name, math::mat44f& value);
	handle bind(const char* name, texture_ptr& value);
	void unbind(handle h);
};

}
}
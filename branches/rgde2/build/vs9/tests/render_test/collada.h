#pragma once

/*
параметры раздел€ютс€ по профил€м, блоками. основной профиль - COMMON



*/
namespace collada
{
	struct image
	{
		const char* id; // is NULL when not in library
		std::string name;
		std::string source; // filename or smth.
	};

	struct material
	{
		const char* id; // is NULL when not in library
		std::string name;
		std::string effect_instance_url;
	};

	struct effect
	{
		const char* id; // is NULL when not in library
		std::string name;

		struct technique
		{
			std::string sid;
			enum type {blinn, constant, lambert, phong};

			type type;

			// params
			struct param
			{
				enum type {
					float_t,
					float2_t,
					float3_t,
					float4_t,
					color_t,					
				};

				std::string name;
				std::string value;

				type type;
			};
 
			typedef std::map<std::string, param> params_t;
			params_t params;
		};

		struct profile
		{
			enum type {bridge, common, cg, gles, glsl, gles2};

			type type;

			typedef std::map<std::string, technique> techniques_t;
			techniques_t techniques;
		};
	};

	struct geometry
	{
		const char* id; // is NULL when not in library
		std::string name;

		struct mesh_t
		{

		};
	};

	struct scene
	{
		enum up_axis
		{
			z_up
		};

		void read(rgde::core::vfs::istream_ptr stream);

		std::string created;
		std::string modified;
		std::string authoring_tool;

		float scale; // to meters

		typedef std::map<std::string, image> library_images_t;
		library_images_t library_images;

		typedef std::map<std::string, material> library_materials_t;
		library_materials_t library_materials;

		typedef std::map<std::string, effect> effects_t;
		effects_t effects;

		typedef std::map<std::string, geometry> geometries_t;
		geometries_t geometries;
	};
}
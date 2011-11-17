#pragma once

/*
	params are separated by profiles
	main profiles is 'COMMON'
*/

namespace collada
{
	struct data3f{
		union {
			struct {float x, y, z;};
			struct {float r, g, b;};
		};
	};

	struct data4f{
		union {
			struct {float x, y, z, w;};
			struct {float r, g, b, w;};
		};
	};

	struct image
	{
		std::string id;
		std::string name;
		std::string source; // filename or other data url.
	};

	struct effect
	{
		struct technique {			
			enum type {blinn, constant, lambert, phong};
			std::string sid;
			type type;
			data4f ambient;
			data4f diffuse;
			data4f specular;
			float shininess;
			data4f reflective;
		};

		struct profile
		{
			enum type {bridge, common, cg, gles, glsl, gles2};
			typedef std::map<std::string, technique> techniques_t;

			type type;			
			techniques_t techniques;
		};

		std::string id;
		std::vector<profile> profiles;
	};

	struct material
	{
		std::string id;
		std::string name;
		std::string effect_instance_url;
		effect*  effect_instance;
	};

	struct mesh
	{
		struct source {
			std::string id;
			std::vector<float> data;
			int stride;
		};

		struct subset_info
		{
			struct input {
				enum semantic_t {
					VERTEX = 0,
					NORMAL,
					TEXCOORD,
					TEXTANGENT,
					TEXBINORMAL,
					INVALID
				};

				std::string source_id;
				semantic_t semantic;
				int offset;
				int set;
			};

			std::string material;
			std::vector<int> indices;
			std::vector<input> inputs;
		};

		typedef std::map<std::string, source> source_library;
		typedef source_library::iterator sources_iter;

		typedef std::vector<subset_info> subsets_store;
		typedef subsets_store::iterator subsets_iter;

		std::string id;
		std::string name;
		source_library sources;
		subsets_store subsets;		
	};

	struct scene
	{
		enum up_axis {
			z_up, y_up
		};

		void read(rgde::core::vfs::istream_ptr stream);

		void load_images_library(xml::node collada_root);
		void load_materials_library(xml::node collada_root);
		void load_effects_library(xml::node collada_root);
		void load_geometries_library(xml::node collada_root);

		std::string created;
		std::string modified;
		std::string authoring_tool;
		up_axis axis;
		float scale; // to meters

		typedef std::map<std::string, image> library_images_t;
		library_images_t library_images;

		typedef std::map<std::string, material> library_materials_t;
		library_materials_t library_materials;

		typedef std::map<std::string, effect> effects_t;
		effects_t effects;

		typedef std::map<std::string, mesh> meshes_t;
		meshes_t meshes;
	};
}
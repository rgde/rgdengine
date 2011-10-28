#pragma once

/*
	params are separated by profiles
	main profiles is 'COMMON'
*/

namespace collada
{
	struct image
	{
		std::string id;
		std::string name;
		std::string source; // filename or other data url.
	};

	struct material
	{
		std::string id;
		std::string name;
		std::string effect_instance_url;
	};

	struct effect
	{
		std::string id;
		std::string name;

		//struct profile {
		//	std::vector<technique> techniques;
		//};

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

		std::vector<profile> profiles;
	};

	struct mesh
	{
		std::string id;
		std::string name;

		struct source
		{
			std::string id;
			std::vector<float> data;
			int stride;
		};

		struct subset_info
		{
			std::string material;

			std::vector<int> indices;

			struct input
			{
				enum semantic_t
				{
					VERTEX = 0,
					NORMAL,
					TEXCOORD,
					INVALID
				};

				std::string source_id;

				semantic_t semantic;
				int offset;
				int set;
			};

			std::vector<input> inputs;
		};

		typedef std::map<std::string, source> source_library;
		typedef source_library::iterator sources_iter;

		typedef std::vector<subset_info> subsets_store;
		typedef subsets_store::iterator subsets_iter;

		source_library sources;
		subsets_store subsets;		
	};

	struct scene
	{
		enum up_axis
		{
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
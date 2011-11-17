#include "stdafx.h"

#include "collada.h"

namespace collada
{
	namespace helpers
	{
		bool read_data(const char* str, data3f& v) {
			if (!str) return false;
			char* buf = const_cast<char*>(str);
			v.x = static_cast<float>(strtod(buf,&buf));
			v.y = static_cast<float>(strtod(buf,&buf));
			v.z = static_cast<float>(strtod(buf,&buf));
			return true;
		}

		bool read_data(const char* str, data4f& v) {
			if (!str) return false;
			char* buf = const_cast<char*>(str);
			v.x = static_cast<float>(strtod(buf,&buf));
			v.y = static_cast<float>(strtod(buf,&buf));
			v.z = static_cast<float>(strtod(buf,&buf));
			v.w = static_cast<float>(strtod(buf,&buf));
			return true;
		}

		bool read_float_array(xml::node array_node, std::vector<float>& out)
		{
			if (!array_node) return false;

			int count = array_node["count"].as_int();
			if (count <= 0)	return false;

			out.resize(count);

			const char* str_data = array_node.first_child().value();
			char* buf = const_cast<char*>(str_data);

			for( int i = 0; i < count; ++i )
			{
				out[i] = static_cast<float>(strtod(buf,&buf));
			}

			return true;
		}

		template<typename T>
		struct str2enum
		{
			const char* str_value;
			T enum_value;
		};

		mesh::subset_info::input::semantic_t semantic_str2enum(const char* str)
		{
			typedef mesh::subset_info::input input;
			static str2enum<input::semantic_t> maping[] = {
				{"VERTEX", input::VERTEX},
				{"NORMAL", input::NORMAL},
				{"TEXCOORD", input::TEXCOORD},
				{"TEXTANGENT", input::TEXTANGENT},
				{"TEXBINORMAL", input::TEXBINORMAL},
				{0, input::INVALID}
			};
			if (strcmp(str, "VERTEX") == 0)  return input::VERTEX;
			else if (strcmp(str, "NORMAL") == 0) return input::NORMAL;
			else if (strcmp(str, "TEXCOORD") == 0) return input::TEXCOORD;
			else if (strcmp(str, "TEXTANGENT") == 0) return input::TEXTANGENT;
			else if (strcmp(str, "TEXBINORMAL") == 0) return input::TEXBINORMAL;
			return input::INVALID;
		}

		bool read_mesh_input(xml::node input_node, mesh::subset_info::input& input)
		{
			const char* semantic = input_node["semantic"].value();
			input.semantic = semantic_str2enum(semantic);

			if (input.semantic == mesh::subset_info::input::INVALID)
				return false; //TODO:

			input.set = input_node["set"] ? input_node["set"].as_int() : -1;
			input.offset = input_node["offset"].as_int();
			input.source_id = input_node["source"].value();

			return true;
		}

		bool read_mesh_source(xml::node source_node, mesh& m)
		{
			std::string source_id = source_node["id"].value();
			mesh::source& source = m.sources["#" + source_id];

			source.id = source_id;
			source.stride = source_node("technique_common")("accessor")["stride"].as_int();				

			xml::node array_node = source_node("float_array");

			bool res = helpers::read_float_array(array_node, source.data);
			assert(res && "Error while reading float array!");
			
			return res;
		}
	}

	struct vertex_id {
		std::vector<int> streams_ids;
	};

	//bool operator < (const vertex_id& l, const vertex_id& r) {		
	//}

	// DOM walker:
	void read_mesh(xml::node geometry_node, mesh& m)
	{
		m.name = geometry_node["name"] ? geometry_node["name"].value() : std::string();	

		xml::node mesh_node = geometry_node("mesh");

		if (!mesh_node)
			return;

		for(xml::node source_node = mesh_node("source");source_node;source_node = source_node.next_sibling("source"))
		{			
			helpers::read_mesh_source(source_node, m);			
		}

		//TODO:
		for(xml::node triangle_node = mesh_node("triangles");triangle_node;triangle_node = triangle_node.next_sibling("triangles"))
		{
			m.subsets.push_back(mesh::subset_info());
			mesh::subset_info& subset = m.subsets.back();

			subset.material = triangle_node["material"].value();
			int count = triangle_node["count"].as_int();

			for(xml::node input_node = triangle_node("input");input_node;input_node = input_node.next_sibling("input"))
			{
				subset.inputs.push_back(mesh::subset_info::input());
				bool res = helpers::read_mesh_input(input_node, subset.inputs.back());
				assert(res && "Error while reading input node data!");
			}

			const char* str_data = triangle_node("p").first_child().value();

			subset.indices.resize(count * subset.inputs.size());

			char* buf = const_cast<char*>(str_data);

			for( size_t i = 0; i < subset.indices.size(); ++i )
			{
				subset.indices[i] = static_cast<float>(strtod(buf,&buf));
			}
		}
	}

	void scene::load_images_library(xml::node collada_node) {
		xml::node library_images_node = collada_node("library_images");

		for(xml::node n = library_images_node("image");n;n = n.next_sibling()) {
			image img;
			img.id = n["id"].value();
			img.name = n["name"].value();
			img.source = n("init_from").first_child().value();

			library_images[img.id] = img;
		}
	}

	void scene::load_materials_library(xml::node collada_node) {


		xml::node library_materials_node = collada_node("library_materials");

		for(xml::node n = library_materials_node("material");n;n = n.next_sibling()) { 
			std::string id = n["id"].value();

			material& mat = library_materials[id];

			mat.id = id;
			mat.name = n["name"].value();;
			mat.effect_instance_url = n("instance_effect")["url"].value();
			if (mat.effect_instance_url.empty()) {
				mat.effect_instance = 0;
				continue;
			}
			// skip 1st char '#'
			std::string effect_name = &(n("instance_effect")["url"].value()[1]);
			effects_t::iterator it = effects.find(effect_name);
			if (it != effects.end()) {
				mat.effect_instance = &(it->second);
			}
		}
	}

	void scene::load_effects_library(xml::node collada_node) {
		for(xml::node n = collada_node("library_effects")("effect"); n; n = n.next_sibling())
		{ 
			std::string id = n["id"].value();
		
			xml::node profile_node = n("profile_COMMON");
			// only handle "COMMON" profiles
			if (!profile_node) {
				//TODO: print some error message as COMMON profile not found for effect
				continue;
			}

			effect& ef = effects[id];
			ef.id = id;

			ef.profiles.push_back(effect::profile());
			effect::profile& profile = ef.profiles.back();
			profile.type = effect::profile::common;

			xml::node tech_node = profile_node("technique").first_child();
			std::string tech_name = tech_node.name();

			effect::technique& tech = profile.techniques[tech_name];
			tech.sid = tech_node["sid"].value();
			tech.type = effect::technique::blinn;

			if (tech_name == "phong")
				tech.type = effect::technique::phong;
			else if (tech_name == "blinn")
				tech.type = effect::technique::blinn;
			else if (tech_name == "lambert")
				tech.type = effect::technique::lambert;
			else 
				tech.type = effect::technique::constant;

			helpers::read_data(tech_node("ambient")("color").child_value(), tech.ambient);
			helpers::read_data(tech_node("diffuse")("color").child_value(), tech.diffuse);
			helpers::read_data(tech_node("specular")("color").child_value(), tech.specular);
			helpers::read_data(tech_node("reflective")("color").child_value(), tech.reflective);
			tech.shininess = atof(tech_node("shininess")("float").child_value());
		}
	}

	void scene::load_geometries_library(xml::node collada_node) {
		xml::node library_geometries_node = collada_node("library_geometries");
		for(xml::node n = library_geometries_node("geometry"); n; n = n.next_sibling())
		{ 
			std::string id = n["id"].value();

			mesh& m = meshes[id];
			m.id = id;

			read_mesh(n, m);
			__asm nop;
		}
	}

	void scene::read(rgde::core::vfs::istream_ptr stream)
	{
		xml::document dae_doc;
		if (!dae_doc.load(stream))
			return;

		//NOTE:
		//() - get node
		//[] - get attribute

		xml::node collada_node = dae_doc("COLLADA");

		xml::node asset_node = collada_node("asset");
		authoring_tool = asset_node("contributor")("authoring_tool").child_value();
		created = asset_node("created").child_value();
		scale = asset_node("unit")["meter"].as_float();

		//TODO:
		//asset_node("up_axis").child_value();
		axis = z_up;

		//xml::node asset_node = collada_node("COLLADA");

		load_images_library(collada_node);
		load_effects_library(collada_node);
		load_materials_library(collada_node);		
		load_geometries_library(collada_node);


		xml::node library_nodes_node = collada_node("library_nodes");
		xml::node library_cameras_node = collada_node("library_cameras");
		xml::node library_visual_scenes_node = collada_node("library_visual_scenes");

		__asm nop;
	}

	//mesh_ptr collada::load_mesh( const TiXmlElement* element, const std::string& geometry_id )
	//{
	//	const TiXmlElement* source_element = element->FirstChildElement( "source" );

	//	mesh_ptr the_mesh( new mesh() );

	//	for( ; source_element; source_element = source_element->NextSiblingElement("source") )
	//	{
	//		string id = GetStringAttribute( source_element, _id );
	//		id = id.substr( geometry_id.size() + 1 );

	//		if( trace_)
	//			std::cout << id <<std::endl;

	//		if( id == "positions" )
	//			load_source( source_element, the_mesh->positions_array_ );
	//		else
	//			if( id == "normals" )
	//				load_source( source_element, the_mesh->normals_array_ );
	//			else
	//				if( id == "map-channel1" )
	//					load_source( source_element, the_mesh->map_channel1_array_ );
	//				else
	//					if( id == "map-channel1-tangents" )
	//						load_source( source_element, the_mesh->map_channel1_tangents_array_ );
	//					else
	//						if( id == "map-channel1-binormals" )
	//							load_source( source_element, the_mesh->map_channel1_binormals_array_ );
	//	}

	//	const TiXmlElement* triangles_element = element->FirstChildElement( "triangles" );
	//	load_triangles( triangles_element, *the_mesh->triangles_.get() );

	//	return the_mesh;
	//}
}
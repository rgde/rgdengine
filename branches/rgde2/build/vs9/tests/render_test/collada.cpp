#include "stdafx.h"

#include "collada.h"

namespace collada
{
	void read_mesh(xml::node geometry_node, mesh& m)
	{
		m.name = geometry_node["name"] ? geometry_node["name"].value() : "";
		m.id = geometry_node["id"] ? geometry_node["id"].value() : "";

		xml::node mesh_node = geometry_node("mesh");

		if (!mesh_node)
			return;

		for(xml::node source_node = mesh_node("source");source_node;source_node = source_node.next_sibling("source"))
		{
			std::string source_id = source_node["id"].value();

			mesh::source& source = m.sources["#" + source_id];
			source.id = source_id;
			source.stride = source_node("technique_common")("accessor")["stride"].as_int();				

			xml::node array_node = source_node("float_array");

			if (!array_node)
				continue; // TODO: add diagnostics
                         
			std::string array_id = array_node["id"].value();			

			const char* str_data = array_node.first_child().value();
			int count = array_node["count"].as_int();

			if (count <= 0)
				return; // TODO: error reporting

			source.data.resize(count);

			char* buf = const_cast<char*>(str_data);

			for( int i = 0; i < count; ++i )
			{
				source.data[i] = static_cast<float>(strtod(buf,&buf));
			}	
		}

		for(xml::node triangle_node = mesh_node("triangles");triangle_node;triangle_node = triangle_node.next_sibling("triangles"))
		{
			m.subsets.push_back(mesh::subset_info());
			mesh::subset_info& subset = m.subsets.back();

			subset.material = triangle_node["material"].value();
			int count = triangle_node["count"].as_int();

			for(xml::node input_node = triangle_node("input");input_node;input_node = input_node.next_sibling("input"))
			{
				subset.inputs.push_back(mesh::subset_info::input());
				mesh::subset_info::input& input = subset.inputs.back();

				std::string semantic = input_node["semantic"].value();

				if (semantic == "VERTEX")
					input.semantic = mesh::subset_info::input::VERTEX;
				else if (semantic == "NORMAL")
					input.semantic = mesh::subset_info::input::NORMAL;
				else if (semantic == "TEXCOORD")
					input.semantic = mesh::subset_info::input::TEXCOORD;
				else
					return; //TODO:

				input.set = input_node["set"] ? input_node["set"].as_int() : -1;
				input.offset = input_node["offset"].as_int();
				input.source_id = input_node["source"].value();
			}

			const char* str_data = triangle_node("p").first_child().value();

			subset.indices.resize(count * subset.inputs.size());

			char* buf = const_cast<char*>(str_data);

			for( int i = 0; i < subset.indices.size(); ++i )
			{
				subset.indices[i] = static_cast<float>(strtod(buf,&buf));
			}

			__asm nop;
		}

		//bool res = get_source_accessor_params( element, accessor_count, accessor_stride );

		//TiXmlHandle source_handle = TiXmlHandle( const_cast<TiXmlElement*>(element) );
		//const TiXmlElement* accessor_element = source_handle.FirstChild( "technique_common" ).FirstChild( "accessor" ).ToElement();
		//if( accessor_element )
		//{
		//	accessor_element->Attribute( count_attribute, &count );
		//	accessor_element->Attribute( stride_attribute, &stride );
		//	return true;
		//}
		//else
		//	return false;

		/*const TiXmlElement* float_array_element = element->FirstChildElement( "float_array" );*/

		//const char* text = float_array_element->GetText();
		//char* buf = const_cast<char*>(text);
		//for( int i = 0; i < accessor_count; ++i )
		//{
		//	vector3& v = container[i];
		//	v[0] = static_cast<float>(strtod(str_data,&str_data));
		//	v[1] = static_cast<float>(strtod(str_data,&str_data));
		//	v[2] = static_cast<float>(strtod(str_data,&str_data));
		//}		

		//const char* text = p_element->GetText();
		//char* buf = const_cast<char*>(text);

		//while( *buf )
		//{
		//	t.indexes_.push_back( strtoul(buf, &buf, 10) );
		//}

		__asm nop;
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

		xml::node asset_node = collada_node("COLLADA");

		if (xml::node library_images_node = collada_node("library_images"))
		{
			for(xml::node n = library_images_node("image");n;n = n.next_sibling())
			{
				std::string id = n["id"].value();
				std::string name = n["name"].value();
				std::string load_path = n("init_from").first_child().value();

				__asm nop;
			}
		}

		if (xml::node library_materials_node = collada_node("library_materials"))
		{
			for(xml::node n = library_materials_node("material");n;n = n.next_sibling())
			{ 
				std::string id = n["id"].value();
				std::string name = n["name"].value();
				std::string effect_instance = n("instance_effect")["url"].value();
				__asm nop;
			}

		}

		if (xml::node library_effects_node = collada_node("library_effects"))
		{
			for(xml::node n = library_effects_node("effect");n;n = n.next_sibling())
			{ 
				std::string id = n["id"].value();
				std::string name = n["name"].value();
				__asm nop;
			}
		}

		if(xml::node library_geometries_node = collada_node("library_geometries"))
		{
			for(xml::node n = library_geometries_node("geometry");n;n = n.next_sibling())
			{ 
				mesh m;
				read_mesh(n, m);
				__asm nop;
			}
		}

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
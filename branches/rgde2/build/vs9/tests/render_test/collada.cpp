#include "stdafx.h"

#include "collada.h"

namespace collada
{
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
				std::string id = n["id"].value();
				std::string name = n["name"].value();

				for(xml::node mesh_node = n("mesh"); mesh_node; mesh_node = mesh_node.next_sibling())
				{
					xml::node source_node = mesh_node("source");
					std::string source_id = source_node["id"].value();

					xml::node array_node = source_node("float_array");
					std::string array_id = array_node["id"].value();
					int cout = array_node["count"].as_int();

					const char* str_data = array_node.first_child().value();

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
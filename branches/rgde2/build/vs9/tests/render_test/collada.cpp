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

}
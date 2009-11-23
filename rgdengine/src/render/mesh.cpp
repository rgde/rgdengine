#include "precompiled.h"

#include <rgde/render/mesh.h>

#include <rgde/scene/scene.h>

#include "../base/exception.h"


namespace render
{
	mesh::mesh()
	: rendererable(10)
	{
		m_render_info.frame = this;//m_frame;
		m_render_info.render_func = boost::bind(&mesh::render, this);
		m_render_info.has_volumes = true;
	}

	mesh::~mesh()
	{
	}

	void mesh::render()
	{
		//base::lwrn << "rendering mesh: " << m_file_name;

		//return;
		// надо вводить понятие SubSet у меша - набор верщин со своим материалом.
		size_t materials_num = m_materials.size();
		materials_list &mats = m_materials;

		if (0 == materials_num)
		{
			m_geometry->render(m_prim_type, m_prim_num);
		}
		else if (1 == materials_num)
		{
			m_geometry->render(m_prim_type, m_prim_num);
		}
		else
		{
			for (sub_meshes::iterator it = m_sub_meshes.begin(); it != m_sub_meshes.end(); ++it)
			{
				IndexedSubMeshInfo &minfo	= *it;
				material_ptr m = mats[minfo.material_index];

				m_geometry->render(m_prim_type, minfo.nBaseVertexIndex, minfo.min_index, minfo.nNumVertices, minfo.nStartIndex, minfo.nPrimitiveCount);
			}
		}
	}

	unsigned int mesh::get_num_verts()const
	{
		unsigned int ret= 0;
		for (sub_meshes::const_iterator it = m_sub_meshes.begin(); it != m_sub_meshes.end(); ++it)
		{
			ret += (*it).nNumVertices;
		}
		if (m_sub_meshes.empty())
			ret = m_vertex_num;
		return ret;
	}

	void mesh::load(const std::string& file_name)
	{
		m_file_name = file_name;
		m_geometry = geometry_ptr(new geometry());

		io::file_system &fs	= io::file_system::get();
		io::path_add_scoped p	("meshes/");

		m_geometry->load(file_name);

		m_geometry->lock_ib();
		m_geometry->unlock_ib();

		m_vertex_num = m_geometry->get_num_verts();
		m_geometry->lock_vb();
		m_geometry->unlock_vb();

		m_prim_type = TriangleList;
		m_prim_num = m_geometry->getIndexNum() / 3;

		m_render_info.bbox = m_geometry->getBBox();
		m_render_info.bsphere = m_geometry->getBSphere();

		//Neonic: octree. Создание локального дерева для меша
		//createLocal( this, (m_render_info.bbox.getMax()-m_render_info.bbox.getMin()) * 0.5f);
	}

	//Neonic: octree
	void mesh::update( bool NeedFullUpdate )
	{
		//if( !getRoot() )
		//{
		////	// calc global AABB from OOB
		////	aaboxf global_aabb;
		////	matrix44f m = m_frame.get()->get_full_tm();
		////	float	Xmax = m_render_info.bbox.getMax()[0],
		////			Ymax = m_render_info.bbox.getMax()[1],
		////			Zmax = m_render_info.bbox.getMax()[2],
		////			Xmin = m_render_info.bbox.getMin()[0],
		////			Ymin = m_render_info.bbox.getMin()[1],
		////			Zmin = m_render_info.bbox.getMin()[2];
		//////(MAX/min)
		////	extendVolume(global_aabb,m*point3f(Xmax,Ymax,Zmax));	// XYZ
		////	extendVolume(global_aabb,m*point3f(Xmax,Ymax,Zmin));	// XYz
		////	extendVolume(global_aabb,m*point3f(Xmax,Ymin,Zmax));	// XyZ
		////	extendVolume(global_aabb,m*point3f(Xmax,Ymin,Zmin));	// Xyz
		////	extendVolume(global_aabb,m*point3f(Xmin,Ymax,Zmax));	// xYZ
		////	extendVolume(global_aabb,m*point3f(Xmin,Ymax,Zmin));	// xYz
		////	extendVolume(global_aabb,m*point3f(Xmin,Ymin,Zmax));	// xyZ
		////	extendVolume(global_aabb,m*point3f(Xmin,Ymin,Zmin));	// xyz

		//	math::aaboxf global_aabb = m_render_info.bbox;
		//	collision::calcAABB(get_full_tm(),global_aabb);
		//	setAABB(global_aabb);
		//	//Neonic: корректируем динамические меши в пространстве 
		//	if(is_dynamic())
		//		updatePos();
		//	scene::TheScene::get().inject(this);
		//}
		//else 
		//if( NeedFullUpdate )
		//{
		//	math::aaboxf global_aabb = m_render_info.bbox;
		//	collision::calcAABB(get_full_tm(),global_aabb);
		//	setAABB(global_aabb);
		//	scene::TheScene::get().inject(this);
		//}
		//else
		//{
		//	move(get_world_pos()+m_render_info.bsphere.getCenter());
		//}
	};

	void mesh::setEffect(effect_ptr shader)
	{
		m_render_info.shader = shader;
		materials_list::iterator it;
		for (it = m_materials.begin(); it != m_materials.end(); ++it)
		{
			(*it)->setEffect(shader);
		}
	}

	const renderable_info & mesh::get_renderable_info() const
	{
		if (m_materials.size() > 0)
			m_render_info.material = *m_materials.begin();
		else
			m_render_info.material = material_ptr();

		return m_render_info;
	}
}
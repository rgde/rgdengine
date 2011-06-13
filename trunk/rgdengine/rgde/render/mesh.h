#pragma once

#include <rgde/math/transform.h>
#include <rgde/render/manager.h>
#include <rgde/render/geometry.h>

namespace render
{
	class mesh : public math::frame
				, public render::rendererable
	{
	public:
		struct IndexedSubMeshInfo		
		{
			unsigned int material_index;
			unsigned int nBaseVertexIndex;
			unsigned int min_index;
			unsigned int nNumVertices; 
			unsigned int nStartIndex;
			unsigned int nPrimitiveCount;
		};

		typedef indexed_geometry<vertex::MeshVertex, false> geometry;
		typedef boost::shared_ptr<geometry>					geometry_ptr;
		typedef std::vector<material_ptr>					materials_list;
		typedef std::vector<IndexedSubMeshInfo>			sub_meshes;

		mesh();
		~mesh();

		void		  load(const std::string& filename); // load from xml file

		//Neonic: octree
		void			update( bool invalidate_transform =0 );

		geometry_ptr	  getGeometry()				{return m_geometry;}
		materials_list& get_materials()			{return m_materials;}

		primitive_type getPrimitiveType() const	{return m_prim_type;}
		unsigned int  getNumPrimitives() const	{return m_prim_num;}
		unsigned int  get_num_verts()const;

		void setEffect(effect_ptr shader);

	protected:
		virtual const renderable_info&	get_renderable_info() const;
		void			render();

	protected:
		std::string		m_file_name;
		materials_list	m_materials;
		sub_meshes		m_sub_meshes;
		geometry_ptr		m_geometry;
		unsigned int	m_vertex_num;

		unsigned int	m_prim_num;
		primitive_type	m_prim_type;
	};

	typedef boost::intrusive_ptr<mesh> mesh_ptr;
}
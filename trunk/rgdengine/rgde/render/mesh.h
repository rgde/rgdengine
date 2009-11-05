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
			unsigned int nMaterialIndex;
			unsigned int nBaseVertexIndex;
			unsigned int nMinIndex;
			unsigned int nNumVertices; 
			unsigned int nStartIndex;
			unsigned int nPrimitiveCount;
		};

		typedef indexed_geometry<vertex::MeshVertex, false> geometry;
		typedef boost::shared_ptr<geometry>					PGeometry;
		typedef std::vector<material_ptr>						MaterialList;
		typedef std::vector<IndexedSubMeshInfo>			sub_meshes;

		mesh();
		~mesh();

		void		  load(const std::string& file_name); // load from xml file

		//Neonic: octree
		void			update( bool NeedFullUpdate =0 );

		PGeometry	  getGeometry()				{return m_geometry;}
		MaterialList& getMaterials()			{return m_materials;}

		primitive_type getPrimitiveType() const	{return m_ePrimType;}
		unsigned int  getNumPrimitives() const	{return m_nNumPrimitives;}
		unsigned int  get_num_verts()const;

		void setEffect(effect_ptr shader);

	protected:
		virtual const renderable_info&	getRenderableInfo() const;
		void			render();

	protected:
		std::string		m_file_name;
		MaterialList	m_materials;
		sub_meshes		m_sub_meshes;
		PGeometry		m_geometry;
		unsigned int	m_vertex_num;

		unsigned int	m_nNumPrimitives;
		primitive_type	m_ePrimType;
	};

	typedef boost::intrusive_ptr<mesh> mesh_ptr;
}
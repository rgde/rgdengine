#pragma once

#include <rgde/math/mathTransform.h>
#include <rgde/render/renderManager.h>
#include <rgde/render/renderGeometry.h>

#include <rgde/physic/collider.h>

namespace render
{
	class CMesh : public math::CFrame
				, public render::IRendererable
				, public collision::CCollisionObject
	{
	public:
		struct SIndexedSubMeshInfo		
		{
			unsigned int nMaterialIndex;
			unsigned int nBaseVertexIndex;
			unsigned int nMinIndex;
			unsigned int nNumVertices; 
			unsigned int nStartIndex;
			unsigned int nPrimitiveCount;
		};

		typedef TIndexedGeometry<vertex::MeshVertex, false> Geometry;
		typedef boost::shared_ptr<Geometry>					PGeometry;
		typedef std::vector<PMaterial>						MaterialList;
		typedef std::vector<SIndexedSubMeshInfo>			SubMeshes;

		CMesh();
		~CMesh();

		void		  load(const std::string& file_name); // load from xml file

		//Neonic: octree
		void			updateTree( bool NeedFullUpdate =0 );

		PGeometry	  getGeometry()				{return m_geometry;}
		MaterialList& getMaterials()			{return m_materials;}

		PrimitiveType getPrimitiveType() const	{return m_ePrimType;}
		unsigned int  getNumPrimitives() const	{return m_nNumPrimitives;}
		unsigned int  getVertexNum()const;

		void setEffect(PEffect spShader);

	protected:
		virtual const SRenderableInfo&	getRenderableInfo() const;
		void			render();

	protected:
		std::string		m_file_name;
		MaterialList	m_materials;
		SubMeshes		m_sub_meshes;
		PGeometry		m_geometry;
		unsigned int	m_vertex_num;

		unsigned int	m_nNumPrimitives;
		PrimitiveType	m_ePrimType;
	};

	typedef boost::intrusive_ptr<CMesh> PMesh;
}
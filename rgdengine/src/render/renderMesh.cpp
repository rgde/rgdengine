#include "precompiled.h"

#include <rgde/render/renderMesh.h>

#include <rgde/scene/scene.h>

#include "../base/exception.h"


namespace render
{
	Mesh::Mesh()
		: IRendererable(10)
	{
		m_renderInfo.pFrame = this;//m_pFrame;
		m_renderInfo.pRenderFunc = boost::bind(&Mesh::render, this);
		m_renderInfo.bHaveVolumes = true;
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::render()
	{
		//base::lwrn << "rendering mesh: " << m_file_name;

		//return;
		// надо вводить понятие SubSet у меша - набор верщин со своим материалом.
		size_t numMaterials	= m_materials.size();
		MaterialList &mlist	= m_materials;

		if (0 == numMaterials)
		{
			m_geometry->render(m_ePrimType, m_nNumPrimitives);
		}
		else if (1 == numMaterials)
		{
			m_geometry->render(m_ePrimType, m_nNumPrimitives);
		}
		else
		{
			for (SubMeshes::iterator it = m_sub_meshes.begin(); it != m_sub_meshes.end(); ++it)
			{
				IndexedSubMeshInfo &minfo	= *it;
				PMaterial m					= mlist[minfo.nMaterialIndex];

				m_geometry->render(m_ePrimType, minfo.nBaseVertexIndex, minfo.nMinIndex, minfo.nNumVertices, minfo.nStartIndex, minfo.nPrimitiveCount);
			}
		}
	}

	unsigned int Mesh::getVertexNum()const
	{
		unsigned int ret= 0;
		for (SubMeshes::const_iterator it = m_sub_meshes.begin(); it != m_sub_meshes.end(); ++it)
		{
			ret += (*it).nNumVertices;
		}
		if (m_sub_meshes.empty())
			ret = m_vertex_num;
		return ret;
	}

	void Mesh::load(const std::string& file_name)
	{
		m_file_name = file_name;
		m_geometry = PGeometry(new Geometry());

		io::CFileSystem &fs	= io::TheFileSystem::Get();
		io::ScopePathAdd p	("Meshes/");

		m_geometry->load(file_name);

		m_geometry->lockIB();
		m_geometry->unlockIB();

		m_vertex_num = m_geometry->getVertexNum();
		m_geometry->lockVB();
		m_geometry->unlockVB();

		m_ePrimType = PrimTypeTriangleList;
		m_nNumPrimitives = m_geometry->getIndexNum() / 3;

		m_renderInfo.bbox = m_geometry->getBBox();
		m_renderInfo.bsphere = m_geometry->getBSphere();

		//Neonic: octree. Создание локального дерева для меша
		//createLocal( this, (m_renderInfo.bbox.getMax()-m_renderInfo.bbox.getMin()) * 0.5f);
	}

	//Neonic: octree
	void Mesh::updateTree( bool NeedFullUpdate )
	{
		//if( !getRoot() )
		//{
		////	// calc global AABB from OOB
		////	AABoxf global_aabb;
		////	Matrix44f m = m_pFrame.get()->getFullTransform();
		////	float	Xmax = m_renderInfo.bbox.getMax()[0],
		////			Ymax = m_renderInfo.bbox.getMax()[1],
		////			Zmax = m_renderInfo.bbox.getMax()[2],
		////			Xmin = m_renderInfo.bbox.getMin()[0],
		////			Ymin = m_renderInfo.bbox.getMin()[1],
		////			Zmin = m_renderInfo.bbox.getMin()[2];
		//////(MAX/min)
		////	extendVolume(global_aabb,m*Point3f(Xmax,Ymax,Zmax));	// XYZ
		////	extendVolume(global_aabb,m*Point3f(Xmax,Ymax,Zmin));	// XYz
		////	extendVolume(global_aabb,m*Point3f(Xmax,Ymin,Zmax));	// XyZ
		////	extendVolume(global_aabb,m*Point3f(Xmax,Ymin,Zmin));	// Xyz
		////	extendVolume(global_aabb,m*Point3f(Xmin,Ymax,Zmax));	// xYZ
		////	extendVolume(global_aabb,m*Point3f(Xmin,Ymax,Zmin));	// xYz
		////	extendVolume(global_aabb,m*Point3f(Xmin,Ymin,Zmax));	// xyZ
		////	extendVolume(global_aabb,m*Point3f(Xmin,Ymin,Zmin));	// xyz

		//	math::AABoxf global_aabb = m_renderInfo.bbox;
		//	collision::calcAABB(getFullTransform(),global_aabb);
		//	setAABB(global_aabb);
		//	//Neonic: корректируем динамические меши в пространстве 
		//	if(isDynamic())
		//		updatePos();
		//	scene::TheScene::Get().inject(this);
		//}
		//else 
		//if( NeedFullUpdate )
		//{
		//	math::AABoxf global_aabb = m_renderInfo.bbox;
		//	collision::calcAABB(getFullTransform(),global_aabb);
		//	setAABB(global_aabb);
		//	scene::TheScene::Get().inject(this);
		//}
		//else
		//{
		//	move(getGlobalPosition()+m_renderInfo.bsphere.getCenter());
		//}
	};

	void Mesh::setEffect(PEffect spShader)
	{
		m_renderInfo.spShader = spShader;
		MaterialList::iterator it;
		for (it = m_materials.begin(); it != m_materials.end(); ++it)
		{
			(*it)->setEffect(spShader);
		}
	}

	const SRenderableInfo & Mesh::getRenderableInfo() const
	{
		if (m_materials.size() > 0)
			m_renderInfo.spMaterial = *m_materials.begin();
		else
			m_renderInfo.spMaterial = PMaterial();

		return m_renderInfo;
	}
}
/*

@author PC
*/

#include "engine.h"
#include "SkinnedMesh.h"

CSkinnedMesh::CSkinnedMesh(const CSkeleton::Bones& bones)
	: TMesh<SkinnedMeshVertex>(), m_skeleton(bones)
{
	
}

CSkeleton& CSkinnedMesh::getSkeleton()
{
	return m_skeleton;
}

const CSkeleton& CSkinnedMesh::getSkeleton() const
{
	return m_skeleton;
}

void CSkinnedMesh::render()
{
	skin();
/*
	m_ePrimType = render::PrimTypeTriangleList;

	//return;
	// надо вводить понятие SubSet у меша - набор верщин со своим материалом.
	size_t numMaterials	= m_materials.size();
	MaterialList &mlist	= m_materials;

	if (0 == numMaterials)
	{
		m_skinnedGeometry->render(m_ePrimType, getNumPrimitives());
	}
	else if (1 == numMaterials)
	{
		m_skinnedGeometry->render(m_ePrimType, getNumPrimitives());
	}
	else
	{
		for (SubMeshes::iterator it = m_sub_meshes.begin(); it != m_sub_meshes.end(); ++it)
		{
			SIndexedSubMeshInfo &minfo	= *it;
			render::PMaterial m					= mlist[minfo.nMaterialIndex];

			m_skinnedGeometry->render(m_ePrimType, minfo.nBaseVertexIndex, minfo.nMinIndex, minfo.nNumVertices, minfo.nStartIndex, minfo.nPrimitiveCount);
		}
	}
*/
	m_skinnedGeometry->render(render::PrimTypeTriangleList, int(m_skinnedGeometry->getIB().size()/3));
}

void CSkinnedMesh::skin()
{
	m_skeleton.makeTransitionMatrices();

	const Geometry::Vertexes& vertsi = m_geometry->getVB();
	Geometry::Vertexes& vertso = m_skinnedGeometry->lockVB();

	const CSkeleton::Matrices& transitionMatrices = m_skeleton.getTransitionMatrices();

	for(size_t i = 0; i < vertsi.size(); i++)
	{
		const SkinnedMeshVertex& vertexi = vertsi[i];
		SkinnedMeshVertex& vertexo = vertso[i];

		vertexo = vertexi;

		math::Matrix44f temp;
		math::Matrix44f resultMatrix;

		math::zero(resultMatrix);

		for(int i = 0; i < 4; i++)
		{
			math::mult(temp, transitionMatrices[vertexi.indices[i]], vertexi.weights[i]);
			math::add(resultMatrix, resultMatrix, temp);
		}

		math::Vec4f tempv(vertexi.position[0], vertexi.position[1], vertexi.position[2], 1.0f);
		tempv = resultMatrix*tempv;
		vertexo.position = math::Vec3f(tempv[0], tempv[1], tempv[2]);
		vertexo.normal = resultMatrix*vertexi.normal;
	}

	m_skinnedGeometry->unlockVB();
}
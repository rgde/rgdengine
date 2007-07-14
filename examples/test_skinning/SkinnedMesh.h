/*

@author PC
*/
#pragma once

#include "Mesh.h"
#include "Skeleton.h"

typedef vertex::PositionSkinnedNormalColoredTextured2TangentBinorm SkinnedMeshVertex;

class CSkinnedMesh : public TMesh<SkinnedMeshVertex>
{
public:

	CSkinnedMesh(const CSkeleton::Bones& bones);

	CSkeleton&       getSkeleton();
	const CSkeleton& getSkeleton() const;

	PGeometry&	  getSkinnedGeometry()				{return m_skinnedGeometry;}

	virtual void render();

private:

	void skin();

	CSkeleton m_skeleton;

	PGeometry m_skinnedGeometry;

};

typedef boost::intrusive_ptr<CSkinnedMesh> PSkinnedMesh;
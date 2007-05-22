#include <rgde/engine.h>

#include "TriangleManager.h"


namespace lines_test
{
	//-----------------------------------------------------------------------------------
	CTriangleManager::CTriangleManager( unsigned nTriangles )
	{
		m_vTriangles.resize(nTriangles);
		reset();
	}
	//-----------------------------------------------------------------------------------
	void CTriangleManager::reset()
	{
		for( tTrianglesIter it = m_vTriangles.begin(); it != m_vTriangles.end(); ++it )
			it->reset();
	}
	//-----------------------------------------------------------------------------------
	void CTriangleManager::update()
	{
		for( tTrianglesIter it = m_vTriangles.begin(); it != m_vTriangles.end(); ++it )
			it->update();
	}
	//-----------------------------------------------------------------------------------
	void CTriangleManager::render()
	{
		for( tTrianglesIter it = m_vTriangles.begin(); it != m_vTriangles.end(); ++it )
			it->render();

		render::CLine3dManager& pLineManager = render::Line3dManager::Get();
		pLineManager.addQuad( math::Vec3f(0,0,0), math::Vec2f(40,40), 0 );
	}
}
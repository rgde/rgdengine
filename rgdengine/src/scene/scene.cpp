#include "precompiled.h"

#include <rgde/scene/scene.h>

#include <rgde/math/mathTransform.h>

namespace scene
{
	CScene::CScene(): m_pRoot( new math::CFrame )
	{
	}

	CScene::~CScene(){}

	math::PFrame& CScene::getRootFrame()
	{
		return m_pRoot;
	}

	void CScene::addManager(const PSceneManager& manager)
	{
		m_Managers.push_back(manager);
	}

	void CScene::removeManager( const PSceneManager& manager )
	{
		m_Managers.remove( manager );
	}

	void CScene::debugDraw( )
	{
		auxDraw( m_pRoot );
	}

	void CScene::auxDraw( math::PFrame frame )
	{
		//frame->debugDraw();
		//for( math::CFrame::ChildrenList::iterator it = frame->getChildren().begin(); it != frame->getChildren().end(); ++it)
		//{
		//	auxDraw( (*it) );
		//}
	}

	void CScene::toStream(io::IWriteStream& wf) const
	{
		wf << *m_pRoot;
		// manager saving
	}

	void CScene::fromStream(io::IReadStream& rf)
	{
		rf >> *m_pRoot;
		// manager loading
	}
}
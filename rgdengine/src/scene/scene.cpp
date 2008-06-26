#include "precompiled.h"

#include <rgde/scene/scene.h>

#include <rgde/math/transform.h>

namespace scene
{
	Scene::Scene(): m_pRoot( new math::Frame )
	{
	}

	Scene::~Scene(){}

	math::PFrame& Scene::getRootFrame()
	{
		return m_pRoot;
	}

	void Scene::addManager(const PSceneManager& manager)
	{
		m_Managers.push_back(manager);
	}

	void Scene::removeManager( const PSceneManager& manager )
	{
		m_Managers.remove( manager );
	}

	void Scene::debugDraw( )
	{
		auxDraw( m_pRoot );
	}

	void Scene::auxDraw( math::PFrame frame )
	{
		//frame->debugDraw();
		//for( math::Frame::ChildrenList::iterator it = frame->getChildren().begin(); it != frame->getChildren().end(); ++it)
		//{
		//	auxDraw( (*it) );
		//}
	}

	void Scene::toStream(io::IWriteStream& wf) const
	{
		wf << *m_pRoot;
		// manager saving
	}

	void Scene::fromStream(io::IReadStream& rf)
	{
		rf >> *m_pRoot;
		// manager loading
	}
}
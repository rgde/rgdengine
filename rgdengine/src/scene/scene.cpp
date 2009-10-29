#include "precompiled.h"

#include <rgde/scene/scene.h>

#include <rgde/math/transform.h>

namespace scene
{
	Scene::Scene(): m_pRoot( new math::Frame )
	{
	}

	Scene::~Scene(){}

	math::frame_ptr& Scene::getRootFrame()
	{
		return m_pRoot;
	}

	void Scene::addManager(const scene_manager_ptr& manager)
	{
		m_Managers.push_back(manager);
	}

	void Scene::removeManager( const scene_manager_ptr& manager )
	{
		m_Managers.remove( manager );
	}

	void Scene::debugDraw( )
	{
		auxDraw( m_pRoot );
	}

	void Scene::auxDraw( math::frame_ptr frame )
	{
		//frame->debugDraw();
		//for( math::Frame::children_list::iterator it = frame->get_children().begin(); it != frame->get_children().end(); ++it)
		//{
		//	auxDraw( (*it) );
		//}
	}

	void Scene::toStream(io::write_stream& wf) const
	{
		wf << *m_pRoot;
		// manager saving
	}

	void Scene::fromStream(io::read_stream& rf)
	{
		rf >> *m_pRoot;
		// manager loading
	}
}
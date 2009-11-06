#include "precompiled.h"

#include <rgde/scene/scene.h>

#include <rgde/math/transform.h>

namespace scene
{
	Scene::Scene(): m_root( math::frame::create() )
	{
	}

	Scene::~Scene(){}

	math::frame_ptr& Scene::get_root()
	{
		return m_root;
	}

	void Scene::add_manager(const scene_manager_ptr& manager)
	{
		m_managers.push_back(manager);
	}

	void Scene::removeManager( const scene_manager_ptr& manager )
	{
		m_managers.remove( manager );
	}

	void Scene::debug_draw( )
	{
		aux_draw( m_root );
	}

	void Scene::aux_draw( math::frame_ptr frame )
	{
		//frame->debug_draw();
		//for( math::frame::children_list::iterator it = frame->get_children().begin(); it != frame->get_children().end(); ++it)
		//{
		//	aux_draw( (*it) );
		//}
	}

	void Scene::to_stream(io::write_stream& wf) const
	{
		wf << *m_root;
		// manager saving
	}

	void Scene::from_stream(io::read_stream& rf)
	{
		rf >> *m_root;
		// manager loading
	}
}
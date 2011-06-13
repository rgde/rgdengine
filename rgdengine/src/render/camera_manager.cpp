#include "precompiled.h"

#include <rgde/render/camera_manager.h>
#include <rgde/render/render_device.h>

#include <rgde/math/camera.h>

namespace render
{
	namespace functors
	{
		struct priority_sorter_less
		{
			bool operator()(math::camera_ptr &c1, math::camera_ptr &c2)
			{
				return c1->priority() < c2->priority() ? true : false;
			}
		};
	}

	camera_manager::camera_manager()
	{
	}

	camera_manager::~camera_manager()
	{
	}

	void camera_manager::add(const math::camera_ptr& c)
	{
		m_cameras.push_back(c);
	}

	void camera_manager::remove(const math::camera_ptr& c)
	{
		m_cameras.remove(c);
	}

	void camera_manager::clear()
	{
		m_cameras.clear();
	}

	void camera_manager::sort()
	{
		m_cameras.sort(functors::priority_sorter_less());
	}

	camera_manager::camera_it camera_manager::begin()
	{
		return m_cameras.begin();
	}
	
	camera_manager::camera_it camera_manager::end()
	{
		return m_cameras.end();
	}

	void camera_manager::activate(camera_manager::camera_it it)
	{
		//if (m_curCamera)
		//{
		//	// do deactivate, m.b. in future...
		//}

		//m_curCamera = *it;
		(*it)->activate();
		render::render_device::get().camera(*it);
	}
}
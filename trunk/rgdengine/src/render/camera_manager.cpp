#include "precompiled.h"

#include <rgde/render/camera_manager.h>
#include <rgde/render/device.h>

#include <rgde/math/camera.h>

namespace render
{
	namespace functors
	{
		struct priority_sorter_less
		{
			bool operator()(math::camera_ptr &c1, math::camera_ptr &c2)
			{
				return c1->get_priority() < c2->get_priority() ? true : false;
			}
		};
	}

	camera_manager::camera_manager()
	{
	}

	camera_manager::~camera_manager()
	{
	}

	void camera_manager::add_camera(const math::camera_ptr& c)
	{
		m_lCameras.push_back(c);
	}

	void camera_manager::removeCamera(const math::camera_ptr& c)
	{
		m_lCameras.remove(c);
	}

	void camera_manager::clear()
	{
		m_lCameras.clear();
	}

	void camera_manager::sort()
	{
		m_lCameras.sort(functors::priority_sorter_less());
	}

	camera_manager::CameraListIterator camera_manager::begin()
	{
		return m_lCameras.begin();
	}
	
	camera_manager::CameraListIterator camera_manager::end()
	{
		return m_lCameras.end();
	}

	void camera_manager::set_camera(camera_manager::CameraListIterator it)
	{
		//if (m_curCamera)
		//{
		//	// do deactivate, m.b. in future...
		//}

		//m_curCamera = *it;
		(*it)->activate();
		render::TheDevice::get().set_camera(*it);
	}
}
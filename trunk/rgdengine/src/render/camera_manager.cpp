#include "precompiled.h"

#include <rgde/render/camera_manager.h>
#include <rgde/render/device.h>

#include <rgde/math/camera.h>

namespace render
{
	namespace functors
	{
		struct SPrioritySorter_Less
		{
			bool operator()(math::camera_ptr &c1, math::camera_ptr &c2)
			{
				return c1->get_priority() < c2->get_priority() ? true : false;
			}
		};
	}

	CameraManager::CameraManager()
	{
	}

	CameraManager::~CameraManager()
	{
	}

	void CameraManager::addCamera(const math::camera_ptr& c)
	{
		m_lCameras.push_back(c);
	}

	void CameraManager::removeCamera(const math::camera_ptr& c)
	{
		m_lCameras.remove(c);
	}

	void CameraManager::clear()
	{
		m_lCameras.clear();
	}

	void CameraManager::sortCameras()
	{
		m_lCameras.sort(functors::SPrioritySorter_Less());
	}

	CameraManager::CameraListIterator CameraManager::begin()
	{
		return m_lCameras.begin();
	}
	
	CameraManager::CameraListIterator CameraManager::end()
	{
		return m_lCameras.end();
	}

	void CameraManager::setCamera(CameraManager::CameraListIterator it)
	{
		//if (m_curCamera)
		//{
		//	// do deactivate, m.b. in future...
		//}

		//m_curCamera = *it;
		(*it)->activate();
		render::TheDevice::get().setCurentCamera(*it);
	}
}
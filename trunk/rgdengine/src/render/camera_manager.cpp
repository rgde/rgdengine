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
			bool operator()(math::PCamera &c1, math::PCamera &c2)
			{
				return c1->getPriority() < c2->getPriority() ? true : false;
			}
		};
	}

	CameraManager::CameraManager()
	{
	}

	CameraManager::~CameraManager()
	{
	}

	void CameraManager::addCamera(const math::PCamera& c)
	{
		m_lCameras.push_back(c);
	}

	void CameraManager::removeCamera(const math::PCamera& c)
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
		render::TheDevice::Get().setCurentCamera(*it);
	}
}
#include "precompiled.h"

#include <rgde/scene/distance_trigger.h>

#include <rgde/math/transform.h>
#include <rgde/math/camera.h>

#include <rgde/render/device.h>

namespace scene
{
	camera_trigger::camera_trigger()
		: m_distance(0)
	{
	}

	void camera_trigger::init(float distance, const math::frame_ptr &frame)
	{
		m_frame = frame; 
		m_distance = distance;
	}

	camera_trigger::camera_trigger(float fDistance, const math::frame_ptr &pFrame)
		: m_frame(pFrame),
		  m_distance(fDistance)
	{
	}

	void camera_trigger::update(float dt)
	{
		if (!m_frame)
			return;

		math::camera_ptr pCam = render::TheDevice::get().get_curent_camera();

		if (!pCam)
			return;

		float fDistance	= math::length<float, 3>(pCam->getPosition() - m_frame->getGlobalPosition());

		bool isCameraInside	= fDistance <= m_distance;

		if (isCameraInside != m_bIsTriggered)
		{
			m_bIsTriggered = isCameraInside;
			trigger(m_bIsTriggered);
		}
	}
}
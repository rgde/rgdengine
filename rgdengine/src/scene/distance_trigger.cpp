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

	camera_trigger::camera_trigger(float distance, const math::frame_ptr &frame)
		: m_frame(frame),
		  m_distance(distance)
	{
	}

	void camera_trigger::update(float dt)
	{
		if (!m_frame)
			return;

		math::camera_ptr pCam = render::TheDevice::get().get_camera();

		if (!pCam)
			return;

		float distance	= math::length<float, 3>(pCam->get_pos() - m_frame->get_world_pos());

		bool isCameraInside	= distance <= m_distance;

		if (isCameraInside != m_is_triggered)
		{
			m_is_triggered = isCameraInside;
			trigger(m_is_triggered);
		}
	}
}
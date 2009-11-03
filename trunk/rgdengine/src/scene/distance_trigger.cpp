#include "precompiled.h"

#include <rgde/math/transform.h>
#include <rgde/math/camera.h>

#include <rgde/scene/distance_trigger.h>

#include <rgde/render/render_device.h>

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

		math::camera_ptr cam = render::TheDevice::get().get_camera();

		if (!cam)
			return;

		float distance	= math::length<float, 3>(cam->get_pos() - m_frame->get_world_pos());

		bool is_inside	= distance <= m_distance;

		if (is_inside != m_is_triggered)
		{
			m_is_triggered = is_inside;
			trigger(m_is_triggered);
		}
	}
}
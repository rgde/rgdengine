/*
IDistanceTrigger is a trigger that triggers when given camera is more close the given
position.
@author PC
*/
#pragma once

#include <rgde/scene/base_trigger.h>

namespace math
{
	typedef boost::intrusive_ptr<class Frame> frame_ptr;
}

namespace scene
{
	class camera_trigger : public IBaseTrigger<bool>
	{
	public:
		camera_trigger();
		camera_trigger(float distance, const math::frame_ptr& frame);

		void init(float distance, const math::frame_ptr& frame);

		void update(float dt);

		//TODO: make get/set interface
	private:
		float         m_distance;
		math::frame_ptr  m_frame;
	};
}
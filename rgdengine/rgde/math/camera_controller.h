//camera_controller.h
#pragma once

#include <rgde/game/dynamic_object.h>

namespace math
{
	class base_camera;
	typedef boost::intrusive_ptr<base_camera> camera_ptr;

	typedef boost::shared_ptr<class base_camera_controller> camera_controller_ptr;

	//camera controller
	class base_camera_controller : public game::dynamic_object, boost::noncopyable
	{
	public:
		virtual ~base_camera_controller() {}

		// camera managing
		void bind(camera_ptr cam)				{m_camera = cam;}
		const camera_ptr& get_camera()	const	{return m_camera;}
		virtual void activate() = 0;

	protected:
		base_camera_controller(camera_ptr cam = camera_ptr()) 
			: m_camera(cam){}

		virtual void update(float dt){}

	protected:
		camera_ptr m_camera;
	};

} //namespace math
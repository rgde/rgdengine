//camera_controller.h
#pragma once

#include <rgde/game/dynamic_object.h>
#include <rgde/math/camera.h>

namespace math
{
	typedef boost::shared_ptr<class base_camera_controller> camera_controller_ptr;

	//котроллер камеры 
	class base_camera_controller : public game::dynamic_object, boost::noncopyable
	{
	public:
		virtual ~base_camera_controller() {}

		//управление камерой
		void set_camera(camera_ptr cam)			{m_pCamera = cam;}
		void resetCamera()						{m_pCamera = camera_ptr();}
		const camera_ptr& get_camera()	const	{return m_pCamera;}
		virtual void activate() = 0;

	protected:
		virtual void update(float dt){}

	protected:
		camera_ptr m_pCamera;
	};

} //namespace math
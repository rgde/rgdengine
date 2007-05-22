//mathCameraController.h
#pragma once

#include <rgde/game/DynamicObject.h>
#include <rgde/math/mathCamera.h>

namespace math
{
	typedef boost::shared_ptr<class BaseCameraController> PCameraController;

	//котроллер камеры 
	class BaseCameraController : public game::IDynamicObject, boost::noncopyable
	{
	public:
		virtual ~BaseCameraController() {}

		//управление камерой
		void setCamera(PCamera cam)			{m_pCamera = cam;}
		void resetCamera()					{m_pCamera = PCamera();}
		const PCamera& getCamera()	const	{return m_pCamera;}
		virtual void activate() = 0;

	protected:
		virtual void update(float dt){}

	protected:
		PCamera m_pCamera;
	};

} //namespace math
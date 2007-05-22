/*
IDistanceTrigger is a trigger that triggers when given camera is more close the given
position.
@author PC
*/
#pragma once

#include <rgde/scene/sceneBaseTrigger.h>

namespace math
{
	typedef boost::intrusive_ptr<class CFrame> PFrame;
}

namespace scene
{
	class CameraTrigger : public IBaseTrigger<bool>
	{
	public:
		CameraTrigger();
		CameraTrigger(float fDistance, const math::PFrame& pFrame);

		void init(float fDistance, const math::PFrame& pFrame);

		void update(float dt);

		//TODO: make get/set interface
	private:
		float         m_fDistance;
		math::PFrame  m_pFrame;
	};
}
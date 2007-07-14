/*

@author PC
*/

#pragma once

#include "AnimationMachine.h"
#include "BaseAnimationClasses.h"

class CAnimationController : public ITimeCounterController
{
public:

	CAnimationController(const CSkeleton::Bones& bones, float fullTime)
		: m_machine(bones), ITimeCounterController(fullTime)
	{
	}
	
	~CAnimationController();

	//Temporally (just to test system)
	void update(float dt);

	void setBlendValue(float value);
	void setBlendingSpeed(float speed);

	void setAnimation1(const PSkeletonAnimation& animation);
	void setAnimation2(const PSkeletonAnimation& animation);

	void setFullTime(float fullTime);

private:

	void setStateNorm(float normTime);

private:

	CAnimationMachine m_machine;
	float m_blendValue;
	float m_blendingSpeed;
	PSkeletonAnimation m_animation1;
	PSkeletonAnimation m_animation2;

};

typedef boost::shared_ptr<CAnimationController> PAnimationController;
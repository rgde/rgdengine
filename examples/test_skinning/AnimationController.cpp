/*

@author PC
*/

#include "engine.h"
#include "AnimationController.h"


CAnimationController::~CAnimationController()
{
}

void CAnimationController::update(float dt)
{
	ITimeCounterController::update(dt);
	m_blendValue += m_blendingSpeed*dt;
	if(m_blendValue > 1.0f)
		m_blendValue = 1.0f;
}

void CAnimationController::setBlendValue(float value)
{
	m_blendValue = value;
}

void CAnimationController::setBlendingSpeed(float speed)
{
	m_blendingSpeed = speed;
}

void CAnimationController::setAnimation1(const PSkeletonAnimation& animation)
{
	m_animation1 = animation;
}

void CAnimationController::setAnimation2(const PSkeletonAnimation& animation)
{
	m_animation2 = animation;
}

void CAnimationController::setFullTime(float fullTime)
{
	ITimeCounterController::setFullTime(fullTime);
}

void CAnimationController::setStateNorm(float normTime)
{
	if(m_animation1 && m_animation2)
	{
		m_machine.setState(m_animation1, normTime, m_blendValue);
		m_machine.addState(m_animation2, normTime, 1.0f - m_blendValue);
		m_machine.apply();
	}
}
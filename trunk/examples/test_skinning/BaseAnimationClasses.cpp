/*

@author PC
*/
#include "engine.h"

#include "BaseAnimationClasses.h"

ITimeCounterController::ITimeCounterController(float fullTime)
	: ITimeController(fullTime)
{
	setEnabled(false);
	setLooped(false);
	setNormTime(0.0f);
	setRate(1.0f);
}

ITimeCounterController::~ITimeCounterController()
{
}

bool ITimeCounterController::getEnabled() const
{
	return m_bEnabled;
}

void ITimeCounterController::setEnabled(bool enabled)
{
	m_bEnabled = enabled;
}

bool ITimeCounterController::getLooped() const
{ 
	return m_bLooped;
}

void ITimeCounterController::setLooped(bool looped)
{ 
	m_bLooped = looped;
}

float ITimeCounterController::getNormTime() const
{ 
	return m_fNormTime; 
}

void ITimeCounterController::setNormTime(float normTime)
{
	m_fNormTime = normTime;

	int periods = int(math::Math::trunc<float>(getNormTime()));

	if(getNormTime() < 0.0f)
		periods--;

	if(periods != 0)
		if(getLooped())
			setNormTime(getNormTime() - periods);
		else
			setEnabled(false);
}

float ITimeCounterController::getTime() const
{ 
	return getNormTime()*getFullTime();
}

void ITimeCounterController::setTime(float time)
{ 
	setNormTime(time/getFullTime()); 
}

float ITimeCounterController::getRate() const
{ 
	return m_fRate; 
}

void ITimeCounterController::setRate(float rate)
{
	if(rate != 0.0f)
		m_fRate = rate;
}

void ITimeCounterController::update(float dt)
{
	if(!getEnabled())
		return;

	setTime(getTime() + dt*getRate());
	updateState();
}

void ITimeCounterController::updateState()
{
	setStateNorm(getNormTime());
}

void ITimeCounterController::play()
{
	setEnabled(true);
}

void ITimeCounterController::start()
{
	reset();
	play();
}

void ITimeCounterController::pause()
{
	setEnabled(false);
}

void ITimeCounterController::stop()
{
	pause();
	reset();
}

void ITimeCounterController::reset()
{
	setNormTime(0.0f);
}
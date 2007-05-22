#include "precompiled.h"

#include <rgde/scene/sceneDistanceTrigger.h>

#include <rgde/math/mathTransform.h>
#include <rgde/math/mathCamera.h>

#include <rgde/render/renderDevice.h>

namespace scene
{
	CameraTrigger::CameraTrigger()
		: m_fDistance(0)
	{
	}

	void CameraTrigger::init(float fDistance, const math::PFrame &pFrame)
	{
		m_pFrame = pFrame; 
		m_fDistance = fDistance;
	}

	CameraTrigger::CameraTrigger(float fDistance, const math::PFrame &pFrame)
		: m_pFrame(pFrame),
		  m_fDistance(fDistance)
	{
	}

	void CameraTrigger::update(float dt)
	{
		if (!m_pFrame)
			return;

		math::PCamera pCam	= render::TheDevice::Get().getCurentCamera();
		if (!pCam)
			return;

		float fDistance		= math::length<float, 3>(pCam->getPosition() - m_pFrame->getGlobalPosition());

		bool isCameraInside	= fDistance <= m_fDistance;

		if (isCameraInside != m_bIsTriggered)
		{
			m_bIsTriggered = isCameraInside;
			trigger(m_bIsTriggered);
		}
	}
}
#include "precompiled.h"

#include "render/renderLines2d.h"

#include "../base/exception.h"

namespace render
{
	//-----------------------------------------------------------------------------------
	CLine2dManager::CLine2dManager(unsigned long uPriority)
		: render::IRendererable(1111),
		  m_Geometry(true)
	{
		base::lmsg << "CLine2dManager::CLine2dManager()";
		m_pVertices = &(m_Geometry.lock());
		m_uPriority = uPriority;
		m_spEffect = IEffect::Create("Line3dManager.fxo");
		m_renderInfo.pRenderFunc = boost::bind(&CLine2dManager::render, this);
	}
	//-----------------------------------------------------------------------------------
	void CLine2dManager::render()
	{
		if (m_pVertices->size() == 0) return;
		m_Geometry.unlock();
		render::IEffect::ITechnique *pTechnique	= m_spEffect->findTechnique("Lines2d");
		pTechnique->begin();
		for (unsigned iPass = 0; iPass < pTechnique->getPasses().size(); ++iPass)
		{
			IEffect::ITechnique::IPass *pPass	= pTechnique->getPasses()[iPass];
			pPass->begin();
			m_Geometry.render(PrimTypeLineList);
			pPass->end();
		}
		pTechnique->end();

		// Сразу после отрисовки линий выносим все линии
		m_pVertices = &(m_Geometry.lock());
		m_pVertices->resize(0);
	}
	//-----------------------------------------------------------------------------------
	void CLine2dManager::addLine(const math::Vec2f &vPoint1, const math::Vec2f vPoint2, math::Color color)
	{
		math::Vec2f screenSize(800, 600); //virtual screen size
		math::Vec2f vFrontBufferSize= render::TheDevice::Get().getBackBufferSize();
		math::Vec2f m_vScale		= vFrontBufferSize / screenSize;
		m_pVertices->push_back(Point(vPoint1 * m_vScale, color));
		m_pVertices->push_back(Point(vPoint2 * m_vScale, color));
	}
} //~ namespace utility
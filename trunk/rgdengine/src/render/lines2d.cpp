#include "precompiled.h"

#include <rgde/render/lines2d.h>

#include "../base/exception.h"

namespace render
{
	//-----------------------------------------------------------------------------------
	Line2dManager::Line2dManager(unsigned long uPriority)
		: render::rendererable(1111),
		  m_Geometry(true)
	{
		base::lmsg << "Line2dManager::Line2dManager()";
		m_pVertices = &(m_Geometry.lock());
		m_uPriority = uPriority;
		m_effect = effect::create("Line3dManager.fx");
		m_renderInfo.render_func = boost::bind(&Line2dManager::render, this);
	}
	//-----------------------------------------------------------------------------------
	void Line2dManager::render()
	{
		if (m_pVertices->size() == 0) return;
		m_Geometry.unlock();
		render::effect::ITechnique *pTechnique	= m_effect->findTechnique("Lines2d");
		pTechnique->begin();
		for (unsigned iPass = 0; iPass < pTechnique->getPasses().size(); ++iPass)
		{
			effect::ITechnique::IPass *pPass	= pTechnique->getPasses()[iPass];
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
	void Line2dManager::add_line(const math::Vec2f &vPoint1, const math::Vec2f vPoint2, math::Color color)
	{
		math::Vec2f screenSize(800, 600); //virtual screen size
		math::Vec2f vFrontBufferSize= render::TheDevice::get().getBackBufferSize();
		math::Vec2f m_vScale		= vFrontBufferSize / screenSize;
		m_pVertices->push_back(Point(vPoint1 * m_vScale, color));
		m_pVertices->push_back(Point(vPoint2 * m_vScale, color));
	}
} //~ namespace utility
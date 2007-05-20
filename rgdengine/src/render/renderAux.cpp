#pragma once

#include "precompiled.h"

#include "render/renderAux.h"
#include "render/renderLines3d.h"

#include "../base/exception.h"

namespace render 
{

	CBone::CBone():render::IRendererable(1)
	{
		m_renderInfo.pFrame = this;
		m_renderInfo.pRenderFunc = boost::bind( &CBone::render, this );
		m_renderInfo.bHaveVolumes = false;
	};	

	void CBone::render()
	{
		const math::Matrix44f &m = getFullTransform();
		math::Point3f org( m.mData[12], m.mData[13], m.mData[14] );

		render::CLine3dManager& pLine3dManager = render::Line3dManager::Get();		

		for (ChildIterator i = getChildren().begin(); i != getChildren().end(); ++i)
		{
			math::Matrix44f mn	= (*i)->getFullTransform();
			math::Point3f org0(mn.mData[12], mn.mData[13], mn.mData[14]);
			pLine3dManager.addLine(org, org0, math::Red);
		};
	}
}
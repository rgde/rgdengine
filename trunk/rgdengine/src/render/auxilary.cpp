#pragma once

#include "precompiled.h"

#include <rgde/render/auxilary.h>
#include <rgde/render/lines3d.h>

#include "../base/exception.h"

namespace render 
{

	Bone::Bone():render::rendererable(1)
	{
		m_renderInfo.pFrame = this;
		m_renderInfo.pRenderFunc = boost::bind( &Bone::render, this );
		m_renderInfo.bHaveVolumes = false;
	};	

	void Bone::render()
	{
		const math::Matrix44f &m = getFullTransform();
		math::Point3f org( m.mData[12], m.mData[13], m.mData[14] );

		render::Line3dManager& line_manager = render::TheLine3dManager::get();		

		for (ChildIterator i = getChildren().begin(); i != getChildren().end(); ++i)
		{
			math::Matrix44f mn	= (*i)->getFullTransform();
			math::Point3f org0(mn.mData[12], mn.mData[13], mn.mData[14]);
			line_manager.addLine(org, org0, math::Red);
		};
	}
}
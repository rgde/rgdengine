/**--------------------------------------------------------------------------------------
									RGDE Engine
	---------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina
mail:	dxprg@mail.ruDate:	14 may 2005
---------------------------------------------------------------------------------------*/

#include <rgde/engine.h>
#include "Triangle.h"

namespace lines_test
{
	//-----------------------------------------------------------------------------------
	CTriangle::CTriangle()
	{
		reset();
	}
	//-----------------------------------------------------------------------------------
	void CTriangle::reset()
	{
		using math::Math::rangeRandom;		m_vPoints.resize(3);
		for( tPointsIter it = m_vPoints.begin(); it != m_vPoints.end(); ++it )
			it->reset();

		float fStrategy = rangeRandom(0,1);
		if( fStrategy < 0.8f )
		{
			float fColor = rangeRandom(0,1);
			m_Color = math::Color( static_cast<unsigned char> (fColor), 0, 255,
				static_cast<unsigned char> (rangeRandom (0, 255)) );

			float fCColorRange = rangeRandom(0,1);

			m_fCColorRange[0] = (fColor - fCColorRange >= 0) ? fColor - fCColorRange : 0;
			m_fCColorRange[1] = (fColor + fCColorRange <= 1) ? fColor + fCColorRange : 1;
			m_fCColorDelta = rangeRandom(0.01f,0.05f);
		}
		else
		{
			m_Color = math::Color( 128, 0, 255, static_cast<unsigned char> (rangeRandom (0, 255)) );
			m_fCColorRange = math::Vec2f(0,1);
			m_fCColorDelta = 0.01f;
		}
	}
	//-----------------------------------------------------------------------------------
	void CTriangle::update()
	{
		for( tPointsIter it = m_vPoints.begin(); it != m_vPoints.end(); ++it )
			it->update();

		float fColor = static_cast<float>(m_Color.r)/255.0f + m_fCColorDelta;

		if( (fColor < m_fCColorRange[0]) || (fColor > m_fCColorRange[1]) )
		{
			m_fCColorDelta = -m_fCColorDelta;
			fColor += m_fCColorDelta;
		}
		m_Color.r = static_cast<unsigned char>(fColor*255);
	}
	//-----------------------------------------------------------------------------------
	void CTriangle::render()
	{
		render::Line3dManager& pLineManager = render::TheLine3dManager::get();

		for( tPointsIter it = m_vPoints.begin(); it != (m_vPoints.end()-1); ++it )
			pLineManager.addLine( it->get_pos(), (it+1)->get_pos(), m_Color.color );

		pLineManager.addLine( (m_vPoints.end()-1)->get_pos(), m_vPoints.begin()->get_pos(), m_Color.color );
	}
}
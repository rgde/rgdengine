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
	triangle::triangle()
	{
		reset();
	}
	//-----------------------------------------------------------------------------------
	void triangle::reset()
	{
		using math::Math::rangeRandom;		m_points.resize(3);
		for( points_iter it = m_points.begin(); it != m_points.end(); ++it )
			it->reset();

		float fStrategy = rangeRandom(0,1);
		if( fStrategy < 0.8f )
		{
			float fColor = rangeRandom(0,1);
			m_color = math::Color( static_cast<unsigned char> (fColor), 0, 255,
				static_cast<unsigned char> (rangeRandom (0, 255)) );

			float fCColorRange = rangeRandom(0,1);

			m_color_range[0] = (fColor - fCColorRange >= 0) ? fColor - fCColorRange : 0;
			m_color_range[1] = (fColor + fCColorRange <= 1) ? fColor + fCColorRange : 1;
			m_color_delta = rangeRandom(0.01f,0.05f);
		}
		else
		{
			m_color = math::Color( 128, 0, 255, static_cast<unsigned char> (rangeRandom (0, 255)) );
			m_color_range = math::vec2f(0,1);
			m_color_delta = 0.01f;
		}
	}
	//-----------------------------------------------------------------------------------
	void triangle::update()
	{
		for( points_iter it = m_points.begin(); it != m_points.end(); ++it )
			it->update();

		float fColor = static_cast<float>(m_color.r)/255.0f + m_color_delta;

		if( (fColor < m_color_range[0]) || (fColor > m_color_range[1]) )
		{
			m_color_delta = -m_color_delta;
			fColor += m_color_delta;
		}
		m_color.r = static_cast<unsigned char>(fColor*255);
	}
	//-----------------------------------------------------------------------------------
	void triangle::render()
	{
		render::lines3d& line_manager = render::render_device::get().get_lines3d();

		for( points_iter it = m_points.begin(); it != (m_points.end()-1); ++it )
			line_manager.add_line( it->get_pos(), (it+1)->get_pos(), m_color.color );

		line_manager.add_line( (m_points.end()-1)->get_pos(), m_points.begin()->get_pos(), m_color.color );
	}
}
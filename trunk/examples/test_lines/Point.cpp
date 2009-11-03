/**--------------------------------------------------------------------------------------
									RGDE Engine
	---------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina
mail:	dxprg@mail.ru
Date:	14 may 2005
---------------------------------------------------------------------------------------*/
#include <rgde/engine.h>
#include "Point.h"
namespace lines_test
{
	const float g_range = 20;					///> Расстояние, на которое летают треугольники
	//-----------------------------------------------------------------------------------
	point::point()
	{
		reset();
	}
	//-----------------------------------------------------------------------------------
	void point::reset()
	{
		using namespace math::Math;
		float rd = g_range;					///> range delta
		float vd = 0.2f;						///> velocity delta

		m_pos = math::Vec3f( rangeRandom(-rd, rd), rangeRandom(-rd, rd),0 );
		m_vel = math::Vec3f( rangeRandom(-vd, vd), rangeRandom(-vd, vd),0 );
	}
	//-----------------------------------------------------------------------------------
	void point::update()
	{
		for( unsigned i = 0; i < 3; ++i )
		{
			float pos = m_pos[i];
			float& vel = m_vel[i];

			if( (pos > g_range) || (pos < -g_range) )
				vel = -vel;
		}
		m_pos += m_vel;
	}
}
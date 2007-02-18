/**--------------------------------------------------------------------------------------
									RGDE Engine
	---------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina
mail:	dxprg@mail.ru
Date:	14 may 2005
---------------------------------------------------------------------------------------*/
#include "engine.h"
#include "Point.h"
namespace lines_test
{
	const float g_fRange = 20;					///> Расстояние, на которое летают треугольники
	//-----------------------------------------------------------------------------------
	CPoint::CPoint()
	{
		reset();
	}
	//-----------------------------------------------------------------------------------
	void CPoint::reset()
	{
		using namespace math::Math;
		float rd = g_fRange;					///> range delta
		float vd = 0.2f;						///> velocity delta

		m_vPos = math::Vec3f( rangeRandom(-rd, rd), rangeRandom(-rd, rd),0 );
		m_vVel = math::Vec3f( rangeRandom(-vd, vd), rangeRandom(-vd, vd),0 );
	}
	//-----------------------------------------------------------------------------------
	void CPoint::update()
	{
		for( unsigned i = 0; i < 3; ++i )
		{
			float pos = m_vPos[i];
			float& vel = m_vVel[i];

			if( (pos > g_fRange) || (pos < -g_fRange) )
				vel = -vel;
		}
		m_vPos += m_vVel;
	}
}
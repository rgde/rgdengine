#pragma once

#include "render/renderDevice.h"
#include "render/renderVertices.h"
#include "render/renderGeometry.h"
#include "render/renderEffect.h"
#include "render/renderManager.h"

#include "math/mathTypes3d.h"

#include "base/singelton.h"

namespace render
{
	class CLine2dManager : public IRendererable
	{
	protected:
		CLine2dManager( unsigned long uPriority = 10);
		~CLine2dManager() {};

	public:
		typedef vertex::PositionTransformedColored Point;

		void addLine( const math::Vec2f& vPoint1, const math::Vec2f vPoint2, 
			math::Color color = 0xffffffff );

	protected:
		void render();

	protected:
		PEffect			m_spEffect;
		unsigned long	m_uPriority;			///> Приоритет менеджера линий

		typedef TGeometry<vertex::PositionTransformedColored> Geometry;
		Geometry m_Geometry;					///> Геометрия
		std::vector<vertex::PositionTransformedColored>* m_pVertices;
	};

	typedef base::TSingelton<CLine2dManager> TheLine2dManager;
} //~ namespace utility
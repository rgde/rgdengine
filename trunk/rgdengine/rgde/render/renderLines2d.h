#pragma once

#include <rgde/render/renderDevice.h>
#include <rgde/render/renderVertices.h>
#include <rgde/render/renderGeometry.h>
#include <rgde/render/renderEffect.h>
#include <rgde/render/renderManager.h>

#include <rgde/math/mathTypes3d.h>

#include <rgde/base/singelton.h>

namespace render
{
	class Line2dManager : public IRendererable
	{
	protected:
		Line2dManager( unsigned long uPriority = 10);
		~Line2dManager() {};

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

	typedef base::TSingelton<Line2dManager> TheLine2dManager;
} //~ namespace utility
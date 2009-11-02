#pragma once

#include <rgde/render/device.h>
#include <rgde/render/vertices.h>
#include <rgde/render/geometry.h>
#include <rgde/render/effect.h>
#include <rgde/render/manager.h>

#include <rgde/math/types3d.h>

#include <rgde/base/singelton.h>

namespace render
{
	class Line2dManager : public rendererable
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
		effect_ptr			m_effect;
		unsigned long	m_uPriority;			///> Приоритет менеджера линий

		typedef TGeometry<vertex::PositionTransformedColored> Geometry;
		Geometry m_Geometry;					///> Геометрия
		std::vector<vertex::PositionTransformedColored>* m_pVertices;
	};

	typedef base::singelton<Line2dManager> TheLine2dManager;
} //~ namespace utility
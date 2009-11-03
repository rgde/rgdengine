#pragma once

#include <rgde/render/render_device.h>
#include <rgde/render/vertices.h>
#include <rgde/render/geometry.h>
#include <rgde/render/effect.h>
#include <rgde/render/manager.h>

#include <rgde/math/types3d.h>

#include <rgde/base/singelton.h>

namespace render
{
	class lines2d : public rendererable
	{
	protected:
		lines2d( unsigned long priority = 10);
		~lines2d() {};

	public:
		typedef vertex::position_transformed_colored Point;

		void add_line( const math::Vec2f& point1, const math::Vec2f point2, 
			math::Color color = 0xffffffff );

	protected:
		void render();

	protected:
		effect_ptr			m_effect;
		unsigned long	m_priority;			///> Приоритет менеджера линий

		typedef geometry<vertex::position_transformed_colored> geometry;
		geometry m_geometry;
		std::vector<vertex::position_transformed_colored>* m_vertices;
	};
} //~ namespace utility
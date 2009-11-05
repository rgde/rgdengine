#pragma once

#include <rgde/render/geometry.h>
#include <rgde/render/manager.h>

namespace render
{
	class lines3d : public rendererable
	{
	public:
		explicit lines3d(unsigned long priority = 10);
	
		typedef vertex::position_colored Point;

		void add_line( const math::vec3f& point1, const math::vec3f& point2, 
			const math::Color& color = 0xffffffff );
		void add_box( const math::matrix44f& m, const math::vec3f& size,
			const math::Color& color = 0xffffffff );

		void add_box(const math::matrix44f& m, const math::aaboxf& box,
			const math::Color& color = 0xffffffff );

		void add_box(const math::vec3f& size, const math::Color& color = 0xffffffff );
		void add_box(const math::aaboxf& box, const math::Color& color = 0xffffffff );

		void add_arrow( const math::matrix44f& m, const math::point3f& dir,
			const math::Color& color = 0xffffffff );
		void add_sphere(const math::matrix44f& m, float rad, int angle );
		void add_quad (const math::vec3f& center, const math::vec2f& size, float spin);

	protected:
		void render();

	protected:
		effect_ptr		m_effect;
		unsigned long	m_priority;			///> drawing priority

		typedef geometry<vertex::position_colored> geometry;
		geometry m_geometry;
		std::vector<vertex::position_colored>* m_vertices;
	};
} //~ namespace utility
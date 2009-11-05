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

		void add_line( const math::Vec3f& point1, const math::Vec3f& point2, 
			const math::Color& color = 0xffffffff );
		void add_box( const math::Matrix44f& m, const math::Vec3f& size,
			const math::Color& color = 0xffffffff );

		void add_box(const math::Matrix44f& m, const math::AABoxf& box,
			const math::Color& color = 0xffffffff );

		void add_box(const math::Vec3f& size, const math::Color& color = 0xffffffff );
		void add_box(const math::AABoxf& box, const math::Color& color = 0xffffffff );

		void add_arrow( const math::Matrix44f& m, const math::Point3f& dir,
			const math::Color& color = 0xffffffff );
		void add_sphere(const math::Matrix44f& m, float rad, int angle );
		void add_quad (const math::Vec3f& center, const math::Vec2f& size, float spin);

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
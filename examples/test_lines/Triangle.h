#pragma once

#include "Point.h"

namespace lines_test
{
	class triangle
	{
	protected:
		typedef std::vector<point> points_vector;
		typedef points_vector::iterator points_iter;

	public:
		triangle();

		void reset();
		void update();
		void render();

	protected:
		points_vector m_points;
		math::Color m_color;
		math::Vec2f m_color_range;				///> color variation range
		float m_color_delta;					///> color variation change delta
	};
}
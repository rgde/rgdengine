#pragma once

#include "Triangle.h"

namespace lines_test
{
	class triangle_manager
	{
	protected:
		typedef std::vector<triangle> triangles_vector;
		typedef triangles_vector::iterator triangles_iter;

	public:
		triangle_manager( unsigned num_triangles );

		void reset();
		void update();
		void render();

	protected:
		triangles_vector m_triangles;					///> Треугольники
	};
}
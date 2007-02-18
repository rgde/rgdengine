#pragma once

#include "Triangle.h"

namespace lines_test
{
	class CTriangleManager
	{
	protected:
		typedef std::vector<CTriangle> tTriangles;
		typedef tTriangles::iterator tTrianglesIter;

	public:
		CTriangleManager( unsigned nTriangles );

		void reset();
		void update();
		void render();

	protected:
		tTriangles m_vTriangles;					///> Треугольники
	};
}
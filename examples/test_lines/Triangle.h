#pragma once

#include "Point.h"

namespace lines_test
{

	class CTriangle
	{
	protected:
		typedef std::vector<CPoint> tPoints;
		typedef tPoints::iterator tPointsIter;

	public:
		CTriangle();

		void reset();
		void update();
		void render();

	protected:
		tPoints m_vPoints;						///> Точки
		math::Color m_Color;				///> Цвет
		math::Vec2f m_fCColorRange;				///> Разброс колебаний меняющегося цвета
		float m_fCColorDelta;					///> Приращение меняющегося цвета
	};
}
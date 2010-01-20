#pragma once

#include "size.h"


namespace gui
{
	/*!
	\brief
	Class used as a two dimensional vector (aka a Point)
	*/
	class GUILIB_API Vector2
	{
	public:
		Vector2(void) {}
		Vector2(float x, float y) : m_x(x), m_y(y) {}

		Vector2& operator*=(const Vector2& vec)
		{
			m_x *= vec.m_x;
			m_y *= vec.m_y;

			return *this;
		}

		Vector2& operator/=(const Vector2& vec)
		{
			m_x /= vec.m_x;
			m_y /= vec.m_y;

			return *this;
		}

		Vector2& operator+=(const Vector2& vec)
		{
			m_x += vec.m_x;
			m_y += vec.m_y;

			return *this;
		}

		Vector2& operator-=(const Vector2& vec)
		{
			m_x -= vec.m_x;
			m_y -= vec.m_y;

			return *this;
		}

		Vector2	operator+(const Vector2& vec) const
		{
			return Vector2(m_x + vec.m_x, m_y + vec.m_y);
		}

		Vector2	operator-(const Vector2& vec) const
		{
			return Vector2(m_x - vec.m_x, m_y - vec.m_y);
		}

		Vector2	operator*(const Vector2& vec) const
		{
			return Vector2(m_x * vec.m_x, m_y * vec.m_y);
		}

		bool	operator==(const Vector2& vec) const
		{
			return ((m_x == vec.m_x) && (m_y == vec.m_y));
		}

		bool	operator!=(const Vector2& vec) const
		{
			return !(operator==(vec));
		}

		Size asSize() const     { return Size(m_x, m_y); }

		float m_x, m_y;
	};

	/*!
	\brief
	Point class
	*/
	typedef	Vector2		Point;


	/*!
	\brief
	Class used as a three dimensional vector
	*/
	class GUILIB_API Vector3
	{
	public:
		Vector3(void) {}
		Vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

		float	m_x, m_y, m_z;
	};
}
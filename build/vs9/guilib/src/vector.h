#pragma once

#include "size.h"


namespace gui
{
	/*!
	\brief
	Class used as a two dimensional vector (aka a Point)
	*/
	class  Vector2
	{
	public:
		Vector2() {}
		Vector2(float x, float y) : x(x), y(y) {}

		Vector2& operator*=(const Vector2& vec)
		{
			x *= vec.x;
			y *= vec.y;

			return *this;
		}

		Vector2& operator/=(const Vector2& vec)
		{
			x /= vec.x;
			y /= vec.y;

			return *this;
		}

		Vector2& operator+=(const Vector2& vec)
		{
			x += vec.x;
			y += vec.y;

			return *this;
		}

		Vector2& operator-=(const Vector2& vec)
		{
			x -= vec.x;
			y -= vec.y;

			return *this;
		}

		Vector2	operator+(const Vector2& vec) const
		{
			return Vector2(x + vec.x, y + vec.y);
		}

		Vector2	operator-(const Vector2& vec) const
		{
			return Vector2(x - vec.x, y - vec.y);
		}

		Vector2	operator*(const Vector2& vec) const
		{
			return Vector2(x * vec.x, y * vec.y);
		}

		bool	operator==(const Vector2& vec) const
		{
			return ((x == vec.x) && (y == vec.y));
		}

		bool	operator!=(const Vector2& vec) const
		{
			return !(operator==(vec));
		}

		float x, y;
	};

	/*!
	\brief
	Point class
	*/
	typedef	Vector2	Point;


	/*!
	\brief
	Class used as a three dimensional vector
	*/
	class  Vector3
	{
	public:
		Vector3(void) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		float	x, y, z;
	};
}
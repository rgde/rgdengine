#include "stdafx.h"

#include <gmtl/gmtl.h>
#include <engine/core/math/color.h>


namespace rgde
{
	namespace math
	{
		color::color(const vec3f& v)
		{
			set(v[0], v[1], v[2], 1.0f);
		}

		color::color(const vec4f& v)
		{
			set(v[0], v[1], v[2], v[3]);
		}

		color::color(unsigned long c) 
			: data(c)
		{
		}

		color::color(const color& c) 
			:data(c.data) 
		{
		}

		color::color(uchar r, uchar g, uchar b, uchar a)
		{
			set(r, g, b, a);
		}

		void color::set(uchar r, uchar g, uchar b, uchar a)
		{
			this->a = a;
			this->r = r;
			this->g = g;
			this->b = b;
		}

		void color::set(float r, float g, float b, float a)
		{
			this->a = (uchar)(a*255);
			this->r = (uchar)(r*255);
			this->g = (uchar)(g*255);
			this->b = (uchar)(b*255);
		}

		color& color::operator=(const vec4f& v)
		{
			set(v[0], v[1], v[2], 1.0f);
			return *this;
		}

		color::operator vec4f() const
		{
			return vec4f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
		}

		color operator*( const color& c, float factor )
		{
			color out;
			out.set(c.r * factor, c.g * factor, c.b * factor, c.a * factor);
			return out;
		}

		color operator+( const color& c1, const color& c2 )
		{
			return color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
		}

		color operator-( const color& c1, const color& c2 )
		{
			return color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
		}

	}
}

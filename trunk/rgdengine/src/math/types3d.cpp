#include "precompiled.h"

#include <rgde/math/types3d.h>

namespace math
{
	// Color
	Color::Color(const vec3f& v) 	
	{
		set(v[0], v[1], v[2], 1.0f);
	}

	Color::Color(const vec4f& v)
	{
		set(v[0], v[1], v[2], v[3]);
	}

	Color::Color(uchar _r, uchar _g, uchar _b, uchar _a)
		: r(_r),
		  g(_g),
		  b(_b),
		  a(_a)
	{
	}

	void Color::set(uchar _r, uchar _g, uchar _b, uchar _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	void Color::set(float _r, float _g, float _b, float _a)
	{
		r = (uchar)(255*_r);
		g = (uchar)(255*_g);
		b = (uchar)(255*_b);
		a = (uchar)(255*_a);
	}

	void Color::set(double _r, double _g, double _b, double _a)
	{
		r = (uchar)(255*_r);
		g = (uchar)(255*_g);
		b = (uchar)(255*_b);
		a = (uchar)(255*_a);
	}

	Color& Color::operator=(const math::vec4f& vec)
	{
		set(vec[0], vec[1], vec[2], vec[3]);
		return *this;
	}
    
	Color operator*( const Color& color, float factor )
	{
		Color out;
		out.set(color.r * factor, color.g * factor, color.b * factor, color.a * factor);
		return out;
	}

	Color operator+( const Color& c1, const Color& c2 )
	{
		return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
	}

	Color operator-( const Color& c1, const Color& c2 )
	{
		return Color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
	}

	Rect::Rect(const vec2f& pos, const vec2f& s) 
		: position(pos), size(s)
	{

	}    
}

std::istream & operator >>(std::istream &in, math::Color& color)
{
	std::skipws(in);

	int r, g, b, a;

	in >> r >> g >> b >> a;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;

	return in;
}
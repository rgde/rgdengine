#pragma once

namespace math
{
	struct Color
	{
		Color(const vec3f& v);
		Color(const vec4f& v);

		Color(unsigned long c = 0) : color(c){}
		Color(const Color& c) :color(c.color) {}
		Color(uchar _r, uchar _g, uchar _b, uchar _a);

		void set(uchar _r, uchar _g, uchar _b, uchar _a);
		void set(float _r, float _g, float _b, float _a);
		void set(double _r, double _g, double _b, double _a);

		Color& operator=(const math::vec4f& vec);

		operator vec4f() const {return vec4f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);}

		union
		{
			unsigned int color;
			struct 
			{
				unsigned char b, g, r, a;
			};
		};
	};

	const Color Green	(0,		255,	0,		255);
	const Color Red		(255,	0,		0,		255);
	const Color Blue	(0,		0,		255,	255);
	const Color White	(255,	255,	255,	255);
	const Color Black	(0,		0,		0,		255);
	const Color Grey	(150, 150, 150, 255);
	const Color	DarkGrey(60, 60, 60, 255);


	Color operator*( const Color& color, float factor );
	Color operator+( const Color& c1, const Color& c2 );
	Color operator-( const Color& c1, const Color& c2 );


	//--------------------------------------------------------------------------------------
	// Rect
	struct Rect
	{
		Rect() {}
		Rect(float x, float y, float w, float h)
			: position(x,y)
			, size(w,h)
		{}

		Rect(const vec2f& pos, const vec2f& s);

		const vec2f& get_top_left()	const	{ return position; }
		const vec2f& get_size()		const	{ return size;}
		const vec2f& get_pos()		const	{ return position;}
		vec2f get_top_right()		const	{ return vec2f (x+w, y); }
		vec2f get_bottom_left()		const	{ return vec2f (x, y+h); }
		vec2f get_bottom_right()	const	{ return vec2f (x+w, y+h); }

		bool is_inside(const math::vec2f& point) const
		{
			if (position[0] > point[0] || position[1] > point[1]) 
				return false;
			else if (point[0] > position[0] + size[0]) 
				return false;
			else if (point[1] > position[1] + size[1]) 
				return false;

			return true;
		}

		union
		{
			struct 
			{
				vec2f position;
				vec2f size;
			};
			struct 
			{
				float x,y,w,h;
			};
		};
	};
}

//void operator >> (std::string str, math::Color& color);
std::istream&  operator >> (std::istream& in, math::Color& color);

template<unsigned int Size, typename Type>
void operator >> (std::string& str, math::vec_base<Type, Size>& v)
{
	using boost::tokenizer;
	tokenizer<> tok(str);

	int i = 0;
	for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end() && i < Size; ++beg, ++i)
	{
		v[i] = base::Lexical_cast<Type, std::string>(*beg);
	}
}

template<typename T, int Size>
std::istream& operator>>(std::istream& in, math::vec_base<T, Size>& v)
{
	using boost::tokenizer;
	int i = 0;

	while(i < Size)
	{
		std::string s;
		in >> s;
		
		tokenizer<> tok(s);

		tokenizer<>::iterator beg=tok.begin();
	
		for(; beg!=tok.end() && i < Size; ++beg, ++i)
		{
			v[i] = base::Lexical_cast<T, std::string>(*beg);
		}
	}

	return in;
}

//---------------------------------------------------------------------------------------
template<typename T, int Size>
math::vec_base<T, Size> operator/ (const math::vec_base<T, Size>& a, const math::vec_base<T, Size>& b)
{
	math::vec_base<T, Size> c;
	for (unsigned i = 0; i < Size; ++i)
		c[i] = a[i] / b[i];
	return c;
}

//---------------------------------------------------------------------------------------
template<typename T, int Size>
math::vec_base<T, Size> operator* (const math::vec_base<T, Size>& a, const math::vec_base<T, Size>& b)
{
	math::vec_base<T, Size> c;
	for (unsigned i = 0; i < Size; ++i)
		c[i] = a[i] * b[i];
	return c;
}

//---------------------------------------------------------------------------------------
template<typename T, int Size>
math::vec_base<T, Size> operator+ (const math::vec_base<T, Size>& a, const math::vec_base<T, Size>& b)
{
	math::vec_base<T, Size> c;
	for (unsigned i = 0; i < Size; ++i)
		c[i] = a[i] + b[i];
	return c;
}

//---------------------------------------------------------------------------------------
template<typename T, int Size>
math::vec_base<T, Size> operator- (const math::vec_base<T, Size>& a, const math::vec_base<T, Size>& b)
{
	math::vec_base<T, Size> c;
	for (unsigned i = 0; i < Size; ++i)
		c[i] = a[i] - b[i];
	return c;
}

template<typename T>
std::istream& operator>>(std::istream& in, math::Quat<T>& q)
{
	std::skipws(in);
	in >> q[0];
	in >> q[1];
	in >> q[2];
	in >> q[3];
	return in;
}
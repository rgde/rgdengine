#pragma once

#include "vector.h"
#include <vector>

namespace gui
{
	/*!
	\brief
	Class encapsulating operations on a Rectangle
	*/
	class  Rect
	{
	public:
		Rect() : m_left(0), m_top(0), m_right(0), m_bottom(0) {}
		Rect(float left, float top, float right, float bottom);
		Rect(point pos, Size sz);

		point	getPosition() const		{return point(m_left, m_top);}
		float	getWidth() const		{return m_right - m_left;}
		float	getHeight() const		{return m_bottom - m_top;}
		Size	getSize() const			{return Size(getWidth(), getHeight());}


		void	setPosition(const point& pt);
		void	setWidth(float width)		{m_right = m_left + width;}
		void	setHeight(float height)		{m_bottom = m_top + height;}
		void	setSize(const Size& size)	{setWidth(size.width); setHeight(size.height);}

		Rect	getIntersection(const Rect& rect) const;
		Rect&	offset(const point& pt);
		bool	isPointInRect(const point& pt) const;
		Rect&	constrainSizeMax(const Size& sz);
		Rect&	constrainSizeMin(const Size& sz);

		Rect&	constrainSize(const Size& max_sz, const Size& min_sz);

		Rect&	scale(const Size& size);
		bool	empty(void) { return (getWidth() <= 0.f || getHeight() <= 0.f); }

		/*************************************************************************
		Operators
		*************************************************************************/
		bool	operator==(const Rect& rhs) const
		{
			return ((m_left == rhs.m_left) && (m_right == rhs.m_right) && (m_top == rhs.m_top) && (m_bottom == rhs.m_bottom));
		}

		bool	operator!=(const Rect& rhs) const		{return !operator==(rhs);}

		/*Rect&	operator=(const Rect& rhs);*/

		Rect operator*(float scalar) const      { return Rect(m_left * scalar, m_top * scalar, m_right * scalar, m_bottom * scalar); }
		const Rect& operator*=(float scalar)    { m_left *= scalar; m_top *= scalar; m_right *= scalar; m_bottom *= scalar; return *this; }

		const Rect& operator*=(const Size& sz) { return  scale(sz); }



		/*************************************************************************
		Data Fields
		*************************************************************************/
		float	m_left, m_top, m_right, m_bottom;
	};

	//Rect& Rect::operator=(const Rect& rhs)
	//{
	//	m_left = rhs.m_left;
	//	m_top = rhs.m_top;
	//	m_right = rhs.m_right;
	//	m_bottom = rhs.m_bottom;

	//	return *this;
	//}

	enum Align
	{
		None	= 0,
		Left	= 1 << 0,
		Top		= 1 << 1,
		Right	= 1 << 2,
		Bottom	= 1 << 3,
		HCenter = 1 << 4,
		VCenter = 1 << 5
	};
	typedef std::vector<Align> Alignment;
	typedef unsigned int AlignmentBits;

	Rect StringToArea(const std::string& str);

	Rect StringToRect(const std::string& str);
	std::string RectToString(const Rect& val);

	Align StringToAlign(const std::string& str);
	std::string AlignToString(Align val);

	unsigned int StringToAlignment(const std::string& str);
	std::string AlignmentToString(unsigned int val);
}

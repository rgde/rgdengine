#include "stdafx.h"
#include "rect.h"

namespace gui
{
	/*************************************************************************
	Constructor
	*************************************************************************/
	Rect::Rect(float left, float top, float right, float bottom) :
m_top(top),
m_bottom(bottom),
m_left(left),
m_right(right)
{
}

Rect::Rect(point pos, Size sz) :
m_top(pos.y),
m_bottom(pos.y + sz.height),
m_left(pos.x),
m_right(pos.x + sz.width)
{
}


/*************************************************************************
Return a Rect object that is the intersection of 'this' with 'rect'
*************************************************************************/
Rect Rect::getIntersection(const Rect& rect) const
{
	// check for total exclusion
	if ((m_right > rect.m_left) &&
		(m_left < rect.m_right) &&
		(m_bottom > rect.m_top) &&
		(m_top < rect.m_bottom))
	{
		Rect temp;

		// fill in temp with the intersection
		//temp.m_left = (m_left > rect.m_left) ? m_left : rect.m_left;
		//temp.m_right = (m_right < rect.m_right) ? m_right : rect.m_right;
		//temp.m_top = (m_top > rect.m_top) ? m_top : rect.m_top;
		//temp.m_bottom = (m_bottom < rect.m_bottom) ? m_bottom : rect.m_bottom;

		float left = (m_left > rect.m_left) ? m_left : rect.m_left;
		float right = (m_right < rect.m_right) ? m_right : rect.m_right;
		float top = (m_top > rect.m_top) ? m_top : rect.m_top;
		float bottom = (m_bottom < rect.m_bottom) ? m_bottom : rect.m_bottom;
		
		return Rect(left, top, right, bottom);
	}
	else
	{
		return Rect(0.0f, 0.0f, 0.0f, 0.0f);
	}

}

/*************************************************************************
Apply an offset the the Rect
*************************************************************************/
Rect& Rect::offset(const point& pt)
{
	m_left		+= pt.x;
	m_right		+= pt.x;
	m_top		+= pt.y;
	m_bottom	+= pt.y;
	return *this;
}


/*************************************************************************
Check if a given point is within the Rect
*************************************************************************/
bool Rect::isPointInRect(const point& pt) const
{
	if ((m_left > pt.x) ||
		(m_right <= pt.x) ||
		(m_top > pt.y) ||
		(m_bottom <= pt.y))
	{
		return false;
	}

	return true;
}

/*************************************************************************
Set location of rect retaining current size.
*************************************************************************/
void Rect::setPosition(const point& pt)
{
	Size sz(getSize());

	m_left = pt.x;
	m_top  = pt.y;
	setSize(sz);
}


/*************************************************************************
check the size of the Rect object and if it is bigger than 'sz', 
resize it so it isn't.	
*************************************************************************/
Rect& Rect::constrainSizeMax(const Size& sz)
{
	if (getWidth() > sz.width)
	{
		setWidth(sz.width);
	}

	if (getHeight() > sz.height)
	{
		setHeight(sz.height);
	}

	return *this;
}


/*************************************************************************
check the size of the Rect object and if it is smaller than 'sz',
resize it so it isn't.
*************************************************************************/
Rect& Rect::constrainSizeMin(const Size& sz)
{
	if (getWidth() < sz.width)
	{
		setWidth(sz.width);
	}

	if (getHeight() < sz.height)
	{
		setHeight(sz.height);
	}

	return *this;
}


/*************************************************************************
check the size of the Rect object and if it is bigger than 'max_sz'
or smaller than 'min_sz', resize it so it isn't.
*************************************************************************/
Rect& Rect::constrainSize(const Size& max_sz, const Size& min_sz)
{
	Size curr_sz(getSize());

	if (curr_sz.width > max_sz.width)
	{
		setWidth(max_sz.width);
	}
	else if (curr_sz.width < min_sz.width)
	{
		setWidth(min_sz.width);
	}

	if (curr_sz.height > max_sz.height)
	{
		setHeight(max_sz.height);
	}
	else if (curr_sz.height < min_sz.height)
	{
		setHeight(min_sz.height);
	}

	return *this;
}

Rect& Rect::scale(const Size& size)
{
	const float scaleX = 1 / size.width;
	const float scaleY = 1 / size.height;
	m_left *= scaleX;
	m_right *= scaleX;
	m_top *= scaleY;
	m_bottom *= scaleY;

	return *this;
}

Rect StringToArea(const std::string& str)
{
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;
	sscanf(str.c_str(), "%f %f %f %f", &x, &y, &w, &h);

	return Rect(point(x, y), Size(w, h));
}

Rect StringToRect(const std::string& str)
{
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;
	sscanf(str.c_str(), "%f %f %f %f", &x, &y, &w, &h);

	return Rect(x, y, w, h);
}

std::string RectToString(const Rect& val)
{
	char buff[128] = {0};
	_snprintf(buff, sizeof (buff), "%d %d %d %d", val.m_left, val.m_top, val.m_right - val.m_left, val.m_bottom - val.m_top);

	return std::string(buff);
}

Align StringToAlign(const std::string& str)
{
	if(str == "left" || str == "Left")
		return Left;
	if(str == "top" || str == "Top")
		return Top;
	if(str == "right" || str == "Right")
		return Right;
	if(str == "bottom" || str == "Bottom")
		return Bottom;
	if(str == "hcenter" || str == "HCenter")
		return HCenter;
	if(str == "vcenter" || str == "VCenter")
		return VCenter;

	return None;
}

std::string AlignToString(Align val)
{
	switch(val)
	{
	case Left:
		return "left";
		break;
	case Top:
		return "top";
		break;
	case Right:
		return "right";
		break;
	case Bottom:
		return "bottom";
		break;
	case HCenter:
		return "hcenter";
		break;
	case VCenter:
		return "vcenter";
		break;
	case None:
	default:
		return "none";
		break;
	};
}

unsigned int StringToAlignment(const std::string& str)
{
	unsigned int a = 0;
	if(!str.empty())
	{
		boost::char_separator<char> sep(", |");
		typedef boost::tokenizer<boost::char_separator<char> > aligntokenizer;
		aligntokenizer tok(str, sep);
		aligntokenizer::iterator i = tok.begin();
		aligntokenizer::iterator end = tok.end();
		while(i != end)
		{
			a |= StringToAlign(*i);
			++i;
		}
	}
	return a;
}

std::string AlignmentToString(unsigned int val)
{
	std::string out;

	for(unsigned int i = 0; i < 8; i++)
	{
		unsigned int a = (1 << i) & val;
		if(i > 0)
			out += " | ";
		out += AlignToString((Align)a);
	}

	return out;
}

}

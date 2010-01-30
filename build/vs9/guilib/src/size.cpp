#include "stdafx.h"
#include "size.h"

namespace gui
{
	bool Size::operator==(const Size& other) const
	{
		return width == other.width && height == other.height;
	}

	bool Size::operator!=(const Size& other) const
	{
		return !operator==(other);
	}

	Size operator* ( const Size& lhs,const Size& rhs )
	{
		return Size( lhs.width*rhs.width, lhs.height*rhs.height );
	}

	Size operator/ ( const Size& lhs,const Size& rhs )
	{
		return Size( lhs.width/rhs.width, lhs.height/rhs.height );
	}

	Size StringToSize(const std::string& str)
	{
		int w,h;
		sscanf(str.c_str(), "%d %d", &w, &h);
		return Size((float)w, (float)h);
	}

	std::string SizeToString(const Size& val)
	{
		char buff[128] = {0};
		_snprintf(buff, sizeof (buff), "%d %d", val.width, val.height);
		return std::string(buff);
	}
}


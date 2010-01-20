#include "stdafx.h"
#include "size.h"

namespace gui
{
	bool Size::operator==(const Size& other) const
	{
		return m_width == other.m_width && m_height == other.m_height;
	}


	bool Size::operator!=(const Size& other) const
	{
		return !operator==(other);
	}



	Size operator* ( const Size& lhs,const Size& rhs )
	{
		return Size( lhs.m_width*rhs.m_width, lhs.m_height*rhs.m_height );
	}

	Size operator/ ( const Size& lhs,const Size& rhs )
	{
		return Size( lhs.m_width/rhs.m_width, lhs.m_height/rhs.m_height );
	}

	Size StringToSize(const std::string& str)
	{
		using namespace std;

		int width = 0;
		int height = 0;
		sscanf(str.c_str(), "%d %d", &width, &height);

		return Size((float)width, (float)height);
	}

	std::string SizeToString(const Size& val)
	{
		char buff[128] = {0};
		_snprintf(buff, sizeof (buff), "%d %d", val.m_width, val.m_height);

		return std::string(buff);
	}
}


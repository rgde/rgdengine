#pragma once

namespace gui
{
	/*!
	\brief
	Class that holds the size (width & height) of something.
	*/
	class  Size
	{
	public:
		Size(void) {}
		Size(float width, float height) : m_width(width), m_height(height) {}

		bool operator==(const Size& other) const;
		bool operator!=(const Size& other) const;

		float m_width, m_height;
	};

	Size operator* ( const Size& lhs,const Size& rhs );
	Size operator/ ( const Size& lhs,const Size& rhs );

	bool StringToBool(const std::string& str);
	std::string BoolToString(bool val);	
}

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
		Size(float width = 0, float height = 0) : width(width), height(height) {}

		bool operator==(const Size& other) const;
		bool operator!=(const Size& other) const;

		float width, height;
	};

	Size operator* ( const Size& lhs,const Size& rhs );
	Size operator/ ( const Size& lhs,const Size& rhs );
}

#include "stdafx.h"

#include <gmtl/gmtl.h>
#include <engine/core/math/rect.h>


namespace rgde
{
	namespace math
	{
		rect::rect()
		{

		}

		rect::rect(float x, float y, float w, float h)
			: position(x,y) , size(w,h)
		{

		}

		rect::rect(const vec2f& pos, const vec2f& s)
			: position(pos), size(s)
		{

		}
	}
}
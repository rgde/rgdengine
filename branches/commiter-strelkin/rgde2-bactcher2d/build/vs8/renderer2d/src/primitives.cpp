#include "stdafx.h"
#include "primitives.h"

namespace rgde
{
	namespace render
	{
		namespace primitives_2d
		{
			line_desc::line_desc() {}

			sprite_desc::sprite_desc(): pos(20, 20), size(10, 10), tex_coord(0, 0, 1, 1), spin(0), color(0xffffffff)
			{
			}
		}
	}
}
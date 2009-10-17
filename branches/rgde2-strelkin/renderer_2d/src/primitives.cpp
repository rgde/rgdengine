#include "stdafx.h"
#include "primitives.h"

namespace rgde
{
	namespace render
	{
		namespace primitive
		{
			sprite::sprite(): pos(20, 20), size(10, 10), tex_coord(0, 0, 1, 1), spin(0), texture(NULL)
			{
			}
		}
	}
}
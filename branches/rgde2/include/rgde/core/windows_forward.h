#pragma once

#include <rgde/core/math/forward.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			struct handle {
				union {
					void* vp;
					int ip;
				};

				operator bool() const {return vp ? true : false;}
			};

			class window;
			typedef boost::shared_ptr<window> window_ptr;
		}
	}
}

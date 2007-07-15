#pragma once

#include <boost/shared_ptr.hpp>

namespace rgde
{
	namespace render
	{
		class device;
		typedef boost::shared_ptr<device> device_ptr;

		class font;
		typedef boost::shared_ptr<font> font_ptr;
	}
}
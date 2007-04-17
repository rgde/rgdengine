#pragma once

namespace rgde
{
	namespace render
	{
		template<typename T>
		void safe_release(T*& p)
		{
			if (NULL == p) return;

			p->Release();
			p = NULL;
		}

		class device;
		typedef boost::shared_ptr<device> device_ptr;

		class font;
		typedef boost::shared_ptr<font> font_ptr;
	}
}
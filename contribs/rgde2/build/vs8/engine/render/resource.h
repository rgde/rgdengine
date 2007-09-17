#pragma once

#include <engine/render/device_object.h>

namespace rgde
{
	namespace render
	{
		class resource : public device_object
		{
		public:
			enum type
			{
				surface,
				texture,
				vertexbuffer,
				indexbuffer
			};

			enum pool
			{
				default,
				managed,
				systemmem
			};

			explicit resource(device& dev, bool need_register, type res_type);

			type get_type() const {return m_type;}

		private:
			type	m_type;
		};
	}
}
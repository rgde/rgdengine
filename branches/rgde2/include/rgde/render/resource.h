#pragma once

namespace rgde
{
	namespace render
	{
		class resource
		{
		public:
			enum format
			{
				unknown,
				r8g8b8,
				a8r8g8b8,
				x8r8g8b8,

				uyvy,
				yuy2,

				dxt1,
				dxt2,
				dxt3,
				dxt4,
				dxt5,

				d32,
				d24s8,
				d24x8,
				d16,

				vertex_data,
				index16,
				index32,

				// -- Floating point surface formats --
				// s10e5 formats (16-bits per channel)
				r16f,
				g16r16f,
				a16b16g16r16f,

				// IEEE s23e8 formats (32-bits per channel)
				r32f,
				g32r32f,
				a32b32g32r32f
			};

			enum type
			{
				surface = 1,
				volume,
				texture,
				volume_texture,
				cube_texture,
				vertexbuffer,
				indexbuffer
			};

			enum pool
			{
				default,
				managed,
				systemmem
			};

			inline type		get_type() const {return m_type;}
			inline format	get_format() const {return m_format;}
			inline pool		get_pool() const {return m_pool;}

		protected:
			resource(type res_type, format res_format, pool res_pool);

		protected:
			type	m_type;
			pool	m_pool;
			format	m_format;
		};
	}
}
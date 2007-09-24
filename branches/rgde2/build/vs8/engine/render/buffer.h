#pragma  once 
#include <engine/render/resource.h>

namespace rgde
{
	namespace render
	{
		class buffer : public resource
		{
		public:
			enum format
			{

			};

			enum usage 
			{

			};			

			format get_format() const {return m_format;}

		protected:

		private:
			format m_format;
		};

		class vertex_buffer : public buffer
		{
		public:

		};

		class index_buffer : public buffer
		{
		public:

		};
	}
}
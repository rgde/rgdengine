#pragma once

#include <list>
#include <rgde/render/render_forward.h>

namespace rgde
{
	namespace render
	{
		class device;

		class device_object
		{
		protected:
			explicit device_object(device& dev, bool register_on_lost_reset = false);

		public:			
			virtual ~device_object();

			device& dev() {return m_device;}
			const device& dev() const {return m_device;}

			static size_t objects_num();

			//virtual void on_device_lost() {}
			//virtual void on_device_reset() {}

		private:
			//typedef std::list<device_object*>::iterator obj_handler;

			device& m_device;
			//bool m_is_registered;
			//obj_handler m_handler;

			static unsigned int ms_num_objects;
		};
	}
}
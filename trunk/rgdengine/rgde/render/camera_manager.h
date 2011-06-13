#pragma once

#include <rgde/base/singelton.h>

namespace math
{
	typedef boost::intrusive_ptr<class base_camera> camera_ptr;
}

namespace render
{
	class camera_manager
	{
	protected:
		camera_manager();
		virtual ~camera_manager();		

	public:
		typedef std::list<math::camera_ptr>	cameras_t;
		typedef cameras_t::iterator		camera_it;

		void			clear		();
		void			add	(const math::camera_ptr& camera);
		void			remove(const math::camera_ptr& camera);

		void			sort	();
		void			activate	(camera_it it);
		
		camera_it	begin();
		camera_it	end();

	protected:
		cameras_t					m_cameras;
	};

	typedef base::singelton<camera_manager> TheCameraManager;
}
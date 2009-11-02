#pragma once

#include <rgde/base/singelton.h>

namespace math
{
	typedef boost::intrusive_ptr<class base_camera> camera_ptr;
}

namespace render
{
	class CameraManager
	{
	protected:
		CameraManager();
		virtual ~CameraManager();		

	public:
		typedef std::list<math::camera_ptr>	CameraList;
		typedef CameraList::iterator		CameraListIterator;

		void			clear		();
		void			addCamera	(const math::camera_ptr& camera);
		void			removeCamera(const math::camera_ptr& camera);

		void			sortCameras	();
		void			setCamera	(CameraListIterator it);
		
		CameraListIterator	begin();
		CameraListIterator	end();

		//const math::camera_ptr& getCurCamera() const {return m_curCamera;}

	protected:
		//math::camera_ptr	m_curCamera;
		CameraList					m_lCameras;
	};

	typedef base::singelton<CameraManager> TheCameraManager;
}
#pragma once

#include <rgde/base/singelton.h>

namespace math
{
	typedef boost::intrusive_ptr<class BaseCamera> PCamera;
}

namespace render
{
	class CameraManager
	{
	protected:
		CameraManager();
		virtual ~CameraManager();		

	public:
		typedef std::list<math::PCamera>	CameraList;
		typedef CameraList::iterator		CameraListIterator;

		void			clear		();
		void			addCamera	(const math::PCamera& camera);
		void			removeCamera(const math::PCamera& camera);

		void			sortCameras	();
		void			setCamera	(CameraListIterator it);
		
		CameraListIterator	begin();
		CameraListIterator	end();

		//const math::PCamera& getCurCamera() const {return m_curCamera;}

	protected:
		//math::PCamera	m_curCamera;
		CameraList					m_lCameras;
	};

	typedef base::TSingelton<CameraManager> TheCameraManager;
}
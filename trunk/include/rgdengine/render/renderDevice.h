#pragma once

#include "math/mathCamera.h"
#include "render/renderFont.h"

#include "base/singelton.h"

namespace render
{
	class IDeviceObject;

	class CDevice
	{
	protected:
		CDevice();
		~CDevice();

	public:
		void					onLost();
		void					onReset();

		void					addDeviceObject(IDeviceObject* pObj);
		void					removeDeviceObject(IDeviceObject* pObj);

		void					setCurentCamera(const math::PCamera& cam){m_curCam = cam; }
		const math::PCamera&	getCurentCamera() const {return m_curCam;}

		void					setShaderFlags(unsigned long flag)	{m_shaderFlags = flag;}
		unsigned long			getShaderFlags() const				{return m_shaderFlags;}

		void					addStatistic( unsigned verts, unsigned tris);
		void					resetStats();

		inline unsigned			getTris()	const {return m_triangles;}
		inline unsigned			getVerts()	const {return m_verts;}

		/// Возвращает размер BackBuffer
		math::Vec2f				getBackBufferSize();		

		void					showWiredFloorGrid(float size, unsigned num = 20, const math::Color& color = math::Green);

		float                   getFPS(float absoluteTime) const;
		void                    showFPS(const PFont& font);
		void                    showStatistics(const PFont& font);

        //->
        void                    setClearColor (math::Color color) {m_ClearColor = color;}
        math::Color             getClearColor () {return m_ClearColor;}
        //-<

	protected:
        //->
        math::Color             m_ClearColor;
        //-<

		unsigned long			m_shaderFlags;
		math::PCamera			m_curCam;

		unsigned				m_verts;
		unsigned				m_triangles;

		typedef std::list<IDeviceObject* > DeviceObjects;
		DeviceObjects m_objects;
	};

	typedef base::TSingelton<CDevice> TheDevice;

	class IDeviceObject
	{
	public:
								IDeviceObject();
		virtual					~IDeviceObject();

		virtual void			onLostDevice() = 0;
		virtual void			onResetDevice() = 0;
	};
}
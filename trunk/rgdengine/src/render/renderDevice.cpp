#include "precompiled.h"

#include <rgde/render/renderDevice.h>
#include <rgde/render/renderBinders.h>
#include <rgde/render/renderLines3d.h>

#include <rgde/core/Timer.h>

#include <d3dx9.h>
extern LPDIRECT3DDEVICE9 g_pd3dDevice;


namespace render
{
	CDevice::CDevice() : m_shaderFlags(0)
	{
        m_ClearColor = math::Color(0,0,0,255);
		//base::lmsg << "CDevice::CDevice()";
	}

	struct  _deleter
	{
		void operator()(IDeviceObject *o)
		{
			delete o;
		}
	};

	CDevice::~CDevice()
	{
		//base::lmsg << "CDevice::~CDevice()";
		onLost();
		//std::for_each(m_objects.begin(), m_objects.end(), _deleter());
	}

	struct  _reseter
	{
		void operator()(IDeviceObject *o)
		{
			o->onResetDevice();
		}
	};
	struct  _loster
	{
		void operator()(IDeviceObject *o)
		{
			o->onLostDevice();
		}
	};


	void CDevice::onLost()
	{
		std::for_each(m_objects.begin(), m_objects.end(), _loster());
	}

	void CDevice::onReset()
	{
		std::for_each(m_objects.begin(), m_objects.end(), _reseter());
	}

	void CDevice::addDeviceObject(IDeviceObject *o)
	{
		m_objects.push_back(o);
	}

	void CDevice::removeDeviceObject(IDeviceObject *o)
	{
		m_objects.remove(o);
	}

	void CDevice::addStatistic(unsigned verts, unsigned tris)
	{
		m_verts += verts;
		m_triangles += tris;
	}

	void CDevice::resetStats()
	{
		m_verts = 0;
		m_triangles = 0;
	}

	//--------------------------------------------------------------------------------------
	math::Vec2f CDevice::getBackBufferSize()
	{
		if (NULL == g_pd3dDevice)
			return math::Vec2f(800, 600);

		IDirect3DSurface9 *pBackBufferSurface;
		g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBufferSurface);

		D3DSURFACE_DESC sd;
		pBackBufferSurface->GetDesc(&sd);

		return math::Vec2f(static_cast<float> (sd.Width), static_cast<float> (sd.Height));
	}

	//--------------------------------------------------------------------------------------
	float CDevice::getFPS(float absoluteTime) const
	{
		static float framesPerSecond;
		static float lastTime		= 0.0f;
		static unsigned int frames	= 0;

		float currentTime			= absoluteTime;

		++ frames;

		if (currentTime - lastTime > 0.50f)
		{
			framesPerSecond = frames / (currentTime - lastTime);
			lastTime = currentTime;
			frames = 0;
		}

		return framesPerSecond;
	}

	void CDevice::showWiredFloorGrid(float size, unsigned num, const math::Color &color)
	{
		float hsize	= size / 2;
		float step	= size / num;

		for (uint i = 0; i < num + 1; ++i)
		{
			{
				// X
				float z			= i *step - hsize;
				math::Vec3f v1	(-hsize, z, 0);
				math::Vec3f v2(hsize, z, 0);
				Line3dManager::Get().addLine(v1, v2, color);
			}
			{
				// Y
				float x	= i *step - hsize;
				math::Vec3f v1(x, -hsize, 0);
				math::Vec3f v2(x, hsize, 0);
				Line3dManager::Get().addLine(v1, v2, color);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	void CDevice::showFPS(const PFont& font)
	{
		WCHAR szFPSString[64];
		wsprintf(szFPSString, L"FPS: %d", (int)getFPS(core::TheTimer::Get().getAbsoluteTime()));
		font->render(szFPSString, math::Rect(1, 1, 400, 400), 0xFFFFFFFF, true);
	}

	void CDevice::showStatistics(const PFont& font)
	{
		WCHAR szStatisticsString[512];
		wsprintf(szStatisticsString, L"Tris: %d, Vertices: %d", m_triangles, m_verts);
		font->render(szStatisticsString, math::Rect(1, 19, 400, 400), 0xFFFFFFFF, true);
	}

	IDeviceObject::IDeviceObject()
	{
		//base::lmsg << "IDeviceObject::IDeviceObject()";
		TheDevice::Get().addDeviceObject(this);
		//m_bIsAtachedToDevice = true;
	}

	IDeviceObject::~IDeviceObject()
	{
		//base::lmsg << "IDeviceObject::~IDeviceObject()";
		//if (m_bIsAtachedToDevice)
		TheDevice::Get().removeDeviceObject(this);
	}
}
#include "precompiled.h"

#include <rgde/render/device.h>
#include <rgde/render/binders.h>
#include <rgde/render/lines3d.h>

#include <rgde/core/timer.h>

#include <d3dx9.h>
extern LPDIRECT3DDEVICE9 g_pd3dDevice;


namespace render
{
	Device::Device() : m_shaderFlags(0)
	{
        m_clear_color = math::Color(0,0,0,255);
		//base::lmsg << "Device::Device()";
	}

	struct  _deleter
	{
		void operator()(IDeviceObject *o)
		{
			delete o;
		}
	};

	Device::~Device()
	{
		//base::lmsg << "Device::~Device()";
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


	void Device::onLost()
	{
		std::for_each(m_objects.begin(), m_objects.end(), _loster());
	}

	void Device::onReset()
	{
		std::for_each(m_objects.begin(), m_objects.end(), _reseter());
	}

	void Device::addDeviceObject(IDeviceObject *o)
	{
		m_objects.push_back(o);
	}

	void Device::removeDeviceObject(IDeviceObject *o)
	{
		m_objects.remove(o);
	}

	void Device::addStatistic(unsigned verts, unsigned tris)
	{
		m_verts += verts;
		m_triangles += tris;
	}

	void Device::resetStats()
	{
		m_verts = 0;
		m_triangles = 0;
	}

	//--------------------------------------------------------------------------------------
	math::Vec2f Device::getBackBufferSize()
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
	float Device::getFPS(float absoluteTime) const
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

	void Device::showWiredFloorGrid(float size, unsigned num, const math::Color &color)
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
				TheLine3dManager::get().addLine(v1, v2, color);
			}
			{
				// Y
				float x	= i *step - hsize;
				math::Vec3f v1(x, -hsize, 0);
				math::Vec3f v2(x, hsize, 0);
				TheLine3dManager::get().addLine(v1, v2, color);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	void Device::showFPS(const PFont& font)
	{
		WCHAR szFPSString[64];
		wsprintf(szFPSString, L"FPS: %d", (int)getFPS(core::TheTimer::get().getAbsoluteTime()));
		font->render(szFPSString, math::Rect(1, 1, 400, 400), 0xFFFFFFFF, true);
	}

	void Device::showStatistics(const PFont& font)
	{
		WCHAR szStatisticsString[512];
		wsprintf(szStatisticsString, L"Tris: %d, Vertices: %d", m_triangles, m_verts);
		font->render(szStatisticsString, math::Rect(1, 19, 400, 400), 0xFFFFFFFF, true);
	}

	IDeviceObject::IDeviceObject()
	{
		//base::lmsg << "IDeviceObject::IDeviceObject()";
		TheDevice::get().addDeviceObject(this);
		//m_bIsAtachedToDevice = true;
	}

	IDeviceObject::~IDeviceObject()
	{
		//base::lmsg << "IDeviceObject::~IDeviceObject()";
		//if (m_bIsAtachedToDevice)
		TheDevice::get().removeDeviceObject(this);
	}
}
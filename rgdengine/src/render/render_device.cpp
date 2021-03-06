#include "precompiled.h"

#include <rgde/render/render_device.h>
#include <rgde/render/binders.h>

#include <rgde/render/lines3d.h>
#include <rgde/render/lines2d.h>

#include <rgde/core/timer.h>

#include <d3dx9.h>
extern LPDIRECT3DDEVICE9 g_d3d;


namespace render
{
	render_device* render_device::ms_instance = 0;
	bool render_device::ms_is_created = false;

	render_device&	render_device::get()
	{
		return *ms_instance;
	}

	bool render_device::is_created()
	{
		return ms_is_created;
	}

	render_device::render_device() : m_shaderFlags(0)
	{
        m_clear_color = math::Color(0,0,0,255);
		ms_instance = this;
		ms_is_created = true;
	}

	struct  _deleter
	{
		void operator()(device_object *o)
		{
			delete o;
		}
	};

	render_device::~render_device()
	{
		on_lost();
		//TODO: add assert on m_objects.empty()
		ms_instance = 0;
		ms_is_created = false;
	}

	void render_device::init()
	{
		m_lines2d.reset(new lines2d());
		m_lines3d.reset(new lines3d());
	}

	struct  _reseter
	{
		void operator()(device_object *o)
		{
			o->onResetDevice();
		}
	};
	struct  _loster
	{
		void operator()(device_object *o)
		{
			o->onLostDevice();
		}
	};


	void render_device::on_lost()
	{
		std::for_each(m_objects.begin(), m_objects.end(), _loster());
	}

	void render_device::on_reset()
	{
		std::for_each(m_objects.begin(), m_objects.end(), _reseter());
	}

	void render_device::add_object(device_object *o)
	{
		m_objects.push_back(o);
	}

	void render_device::remove_object(device_object *o)
	{
		m_objects.remove(o);
	}

	void render_device::add_statistics(unsigned verts, unsigned tris)
	{
		m_verts += verts;
		m_triangles += tris;
	}

	void render_device::reset_statistics()
	{
		m_verts = 0;
		m_triangles = 0;
	}

	//--------------------------------------------------------------------------------------
	math::vec2f render_device::getBackBufferSize()
	{
		if (NULL == g_d3d)
			return math::vec2f(800, 600);

		IDirect3DSurface9 *pBackBufferSurface;
		g_d3d->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBufferSurface);

		D3DSURFACE_DESC sd;
		pBackBufferSurface->GetDesc(&sd);

		return math::vec2f(static_cast<float> (sd.Width), static_cast<float> (sd.Height));
	}

	//--------------------------------------------------------------------------------------
	float render_device::get_fps(float abs_time) const
	{
		static float framesPerSecond;
		static float lastTime		= 0.0f;
		static unsigned int frames	= 0;

		float currentTime			= abs_time;

		++ frames;

		if (currentTime - lastTime > 0.50f)
		{
			framesPerSecond = frames / (currentTime - lastTime);
			lastTime = currentTime;
			frames = 0;
		}

		return framesPerSecond;
	}

	void render_device::draw_wired_floor(float size, unsigned num, const math::Color &color)
	{
		float hsize	= size / 2;
		float step	= size / num;

		for (uint i = 0; i < num + 1; ++i)
		{
			{
				// X
				float z	= i *step - hsize;
				math::vec3f v1	(-hsize, 0, z);
				math::vec3f v2(hsize, 0, z);
				get_lines3d().add_line(v1, v2, color);
			}
			{
				// Y
				float x	= i *step - hsize;
				math::vec3f v1(x, 0, -hsize);
				math::vec3f v2(x, 0, hsize);
				get_lines3d().add_line(v1, v2, color);
			}
		}
	}

	lines3d& render_device::get_lines3d()
	{
		return *m_lines3d.get();
	}

	lines2d& render_device::get_lines2d()
	{
		return *m_lines2d.get();
	}

	//--------------------------------------------------------------------------------------
	void render_device::draw_fps(float abs_time, const font_ptr& font)
	{
		WCHAR szFPSString[64];
		wsprintf(szFPSString, L"FPS: %d", (int)get_fps(abs_time));
		font->render(szFPSString, math::Rect(1, 1, 400, 400), 0xFFFFFFFF, true);
	}

	void render_device::showStatistics(const font_ptr& font)
	{
		WCHAR szStatisticsString[512];
		wsprintf(szStatisticsString, L"Tris: %d, Vertices: %d", m_triangles, m_verts);
		font->render(szStatisticsString, math::Rect(1, 19, 400, 400), 0xFFFFFFFF, true);
	}

//////////////////////////////////////////////////////////////////////////

	device_object::device_object()
	{
		//base::lmsg << "device_object::device_object()";
		render_device::get().add_object(this);
		//m_bIsAtachedToDevice = true;
	}

	device_object::~device_object()
	{
		//base::lmsg << "device_object::~device_object()";
		//if (m_bIsAtachedToDevice)
		render_device::get().remove_object(this);
	}
}
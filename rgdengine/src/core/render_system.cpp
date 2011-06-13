#include "precompiled.h"

#include <rgde/core/render_system.h>

#include <rgde/math/types3d.h>
#include <rgde/core/application.h>

#include "render_system_impl.h"

namespace core
{
	render_system::render_system(const application& app, render_device_impl_ptr device)
		: m_app(app)
		, m_render_device(device)
	{

	}

	render_system::~render_system()
	{
	}

	void render_system::save_screen(const std::wstring& filename)
	{
		m_render_device->save_screen(filename);
	}

	void render_system::update() const
	{
		m_render_device->update();
	}

	render::render_device& render_system::get_device()
	{
		return *m_render_device;
	}

	const render::render_device& render_system::get_device() const
	{
		return *m_render_device;
	}

	render_system_ptr render_system::create(const application& app)
	{
		render_device_impl_ptr device(new render_device_impl((HWND)app.get_handle()));
		return render_system_ptr(new render_system(app, device));
	}
}
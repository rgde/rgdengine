#include "precompiled.h"

#include <rgde/core/render_system.h>

#include <rgde/math/types3d.h>
#include <rgde/core/application.h>

#include "render_system_impl.h"

namespace core
{
	render_system::~render_system()
	{
	}

	render_system_ptr render_system::create(const application& app)
	{
		return render_system_ptr(new CDXRenderDevice((HWND)app.get_handle()));
	}
}
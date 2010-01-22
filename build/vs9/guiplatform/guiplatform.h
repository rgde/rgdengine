#pragma once

//struct IDirect3DDevice9;
#include <rgde/render/render_forward.h>

namespace gui
{
	class Renderer;

	namespace rgde_platform
	{
		Renderer* CreateRenderer(rgde::render::device_ptr dev, unsigned buff = 1024);
	}	
}

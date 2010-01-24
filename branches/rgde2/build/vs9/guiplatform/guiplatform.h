#pragma once

//struct IDirect3DDevice9;
#include <rgde/render/render_forward.h>
#include <rgde/core/file_system.h>

namespace gui
{
	class Renderer;

	namespace rgde_platform
	{
		Renderer* CreateRenderer(rgde::render::device& dev, 
			rgde::core::vfs::system& vfs,
			unsigned buff = 1024);
	}	
}

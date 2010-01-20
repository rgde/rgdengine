#pragma once

struct IDirect3DDevice9;

namespace gui
{
	class Renderer;

	namespace win32dx9platform
	{
		Renderer* CreateRenderer(IDirect3DDevice9* dev, unsigned buff = 1024);
	}	
}

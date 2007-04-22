#pragma once

#include "ArcBall.h"

class Application : window
{
public:
	Application();
	~Application();

	void run();

protected:
	virtual core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
	void resize_scene(unsigned int width, unsigned int height);
	bool do_events();

private:
	ArcBall m_arc_ball;
	bool m_active;
	device m_device;
	font m_font;
	IDirect3DVertexBuffer9 *pVertexBuffer;
	D3DXVECTOR3 m_cam_pos;
};

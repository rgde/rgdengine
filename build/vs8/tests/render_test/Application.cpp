#include "stdafx.h"

#include <boost/assign.hpp>

using namespace rgde;
using core::windows::window;
using render::device;
using render::font;
using namespace math;

#include "Application.h"

float g_rot_x = 0;
float g_rot_y = 0;

int old_x = -1;
int old_y = -1;

int clicked_x = 0;
int clicked_y = 0;


using rgde::render::vertex_element;
vertex_element custom_vertex_desc[] = 
{
	{0, 0,  vertex_element::float3,   vertex_element::default_method, vertex_element::position, 0}, 
	{0, 12, vertex_element::color4ub, vertex_element::default_method, vertex_element::color,	0},
	vertex_element::end_element
};


struct tVertex				// Our new vertex struct
{
	float x, y, z;			// 3D position
	ulong color;			// Hex Color Value
};


tVertex cube[24] =			// Vertex Array
{	
	// Front Blue Color
	{-1.0f, 1.0f,-1.0f,  0xF00FF00F },
	{ 1.0f, 1.0f,-1.0f,  0xF00FF00F },
	{-1.0f,-1.0f,-1.0f,  0xF00FF00F },
	{ 1.0f,-1.0f,-1.0f,  0xF00FF00F },
	// Back Orange Color
	{-1.0f, 1.0f, 1.0f,  0xFFF8800F },
	{-1.0f,-1.0f, 1.0f,  0xFFF8800F },
	{ 1.0f, 1.0f, 1.0f,  0xFFF8800F },
	{ 1.0f,-1.0f, 1.0f,  0xFFF8800F },
	// Top Red Color
	{-1.0f, 1.0f, 1.0f,  0xFFF0000F },
	{ 1.0f, 1.0f, 1.0f,  0xFFF0000F },
	{-1.0f, 1.0f,-1.0f,  0xFFF0000F },
	{ 1.0f, 1.0f,-1.0f,  0xFFF0000F },
	// Bottom Yellow Color
	{-1.0f,-1.0f, 1.0f,  0xFFFFF00F },
	{-1.0f,-1.0f,-1.0f,  0xFFFFF00F },
	{ 1.0f,-1.0f, 1.0f,  0xFFFFF00F },
	{ 1.0f,-1.0f,-1.0f,  0xFFFFF00F },
	// Right Blue Color
	{ 1.0f, 1.0f,-1.0f,  0xF0000FFF },
	{ 1.0f, 1.0f, 1.0f,  0xF0000FFF },
	{ 1.0f,-1.0f,-1.0f,  0xF0000FFF },
	{ 1.0f,-1.0f, 1.0f,  0xF0000FFF },
	// Left Violet Color
	{-1.0f, 1.0f,-1.0f,  0xFFF00FFF },
	{-1.0f,-1.0f,-1.0f,  0xFFF00FFF },
	{-1.0f, 1.0f, 1.0f,  0xFFF00FFF },
	{-1.0f,-1.0f, 1.0f,  0xFFF00FFF }
};


Application::Application() 
	: m_active(true),
	window(L"RenderTest"),
	m_device(get_handle()),
	m_font(font::create(m_device, 18, L"Arial", font::heavy)),
	m_arc_ball(640, 480)
{
	show();
	update();

	using namespace rgde::render;
	vertex_declaration_ptr decl = vertex_declaration::create(m_device, custom_vertex_desc, 3);

	m_vb = vertex_buffer::create
						(
							m_device, decl, 
							24*sizeof(tVertex), 
							resource::default, 
							buffer::write_only
						);

	m_device.set_ztest(true);
	m_device.set_cull_mode(rgde::render::cull_none);
	m_device.set_lighting(false);
	m_device.set_alpha_blend(false);
	m_device.set_alpha_test(false);

	void *pVertices = m_vb->lock( 0, sizeof(cube), 0 );
	memcpy( pVertices, cube, sizeof(cube) );
	m_vb->unlock();

	D3DXVECTOR3 cam_pos(-5, 0, 0);
	m_camera.lookAt(cam_pos, (D3DXVECTOR3(1,0,0) + cam_pos), D3DXVECTOR3(0,1,0));
}

Application::~Application()
{
}

void Application::run()
{		
	while( is_created() )
	{
		if( !do_events() && m_active)
		{
			Sleep(10);

			static float rotqube = 0.0f;
			rotqube  += 0.9f;

			D3DXMatrixIdentity(&matWorld);

			m_device.set_transform( rgde::render::view_transform, m_camera.getViewMatrix() );
			m_device.set_transform( rgde::render::world_transform, getWorldMatrix() );	// Set The Transformation

			m_device.frame_begin();
			//m_device.clear(Grey);
			m_device.clear(0x00595979);
			m_device.set_stream_source( 0, m_vb, sizeof(tVertex) );

			m_device.draw( rgde::render::triangle_strip,  0, 2 ); // Draw Front
			m_device.draw( rgde::render::triangle_strip,  4, 2 ); // Draw Back
			m_device.draw( rgde::render::triangle_strip,  8, 2 ); // Draw Top
			m_device.draw( rgde::render::triangle_strip,  12, 2 ); // Draw Bottom
			m_device.draw( rgde::render::triangle_strip,  16, 2 ); // Draw Right
			m_device.draw( rgde::render::triangle_strip,  20, 2 ); // Draw Left

			m_font->render(L"Hello", rect(10,10,100,100),Red, true);
			m_device.frame_end();
			m_device.present();
		}
	}
}

core::windows::result Application::wnd_proc(ushort message, uint wparam, long lparam )
{
	switch (message)
	{
	case WM_ACTIVATE:	// Watch For Window Activate Message
		{
			if (!HIWORD(wparam))// Check Minimization State
				m_active = true;
			else
				m_active = false;

			return 0;
		}
	case WM_KEYDOWN:
		{
			//keys[wParam] = true;
			if (VK_UP == wparam)
			{
				m_cam_pos += D3DXVECTOR3(0.05f,0,0);

			}
			else if (VK_DOWN == wparam)
			{
				m_cam_pos -= D3DXVECTOR3(0.05f,0,0);
			}


			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		SetCapture( get_handle() );
		ShowCursor(FALSE);
		{
			POINT p;
			GetCursorPos(&p);
			clicked_x = p.x;
			clicked_y = p.y;

			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam); 
		}
		return 0;

	case WM_LBUTTONUP:
		{
			SetCursorPos(clicked_x, clicked_y);
			ShowCursor(TRUE);
			ReleaseCapture();
		}
		return 0;


	case WM_KEYUP:
		{
			//keys[wParam] = false;
			return 0;
		}

	case WM_SIZE:
		{
			resize_scene(LOWORD(lparam), HIWORD(lparam));
			return 0;
		}
	case WM_MOUSEWHEEL:
		{
			float delta = (short)HIWORD((DWORD)wparam);//120.0f;
			delta /= 80.0f;
			m_camera.goForward(-delta);
		}
		break;

	case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam); 

			
			if ((old_x != -1 || old_y != -1) && ((wparam & MK_LBUTTON) != 0))
			{
				m_arc_ball.drag(xPos, yPos);
				int dx = xPos - old_x;
				int dy = yPos - old_y;

				m_camera.rotateRight(-dx/100.0f);
				m_camera.rotateUp(dy/100.0f);
			}

			old_x = xPos;
			old_y = yPos;
		}
		break;
	}
	return window::wnd_proc(message, wparam, lparam);
}

void Application::resize_scene(unsigned int width, unsigned int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	m_camera.setProjection(45.0f, (float)width/(float)height, 0.1f, 100.0f);
	m_device.set_transform( rgde::render::projection_transform, m_camera.getProjMatrix());
}

bool Application::do_events()
{
	MSG msg = {0};
	if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		return true;
	}
	return false;
}
#include "stdafx.h"

#include <d3dx9.h>
#include <Windows.h>

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


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)


struct tVertex				// Our new vertex struct
{
	float x, y, z;			// 3D position
	DWORD color;			// Hex Color Value
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
	m_font(m_device, 18, L"Arial", font::Heavy),
	m_arc_ball(640, 480)
{
	show();
	update();
	pVertexBuffer = NULL;

	IDirect3DDevice9* dev = m_device.get_dx_device();

	dev->SetRenderState(D3DRS_ZENABLE,  TRUE ); // Z-Buffer (Depth Buffer)
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // Disable Backface Culling
	dev->SetRenderState(D3DRS_LIGHTING, FALSE); // Disable Light
	dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); // Disable Light
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // Disable Light

	dev->CreateVertexBuffer( 24*sizeof(tVertex),0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &pVertexBuffer, NULL );
	unsigned char *pVertices = NULL;

	pVertexBuffer->Lock( 0, sizeof(cube), (void**)&pVertices, 0 );
	memcpy( pVertices, cube, sizeof(cube) );
	pVertexBuffer->Unlock();

	D3DXVECTOR3 cam_pos(-5, 0, 0);
	m_camera.lookAt(cam_pos, (D3DXVECTOR3(1,0,0) + cam_pos), D3DXVECTOR3(0,1,0));
}

Application::~Application()
{
	pVertexBuffer->Release();
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

			//D3DXMATRIX matWorld;			// Create The World Matrix
			//D3DXMATRIX matTranslation;		// Create a Translation Matrix
			//D3DXMATRIX matRotX;				// Create X Axis Rotation Matrix
			//D3DXMATRIX matRotY;				// Create Y Axis Rotation Matrix
			//D3DXMatrixTranslation( &matTranslation, 0.0f, 0.0f, 0.0f ); 
			//D3DXMatrixRotationZ(&matRotX, D3DXToRadian(g_rot_x));		// Rotate In X Direction
			//D3DXMatrixRotationY(&matRotY, D3DXToRadian(g_rot_y));		// Rotate In Z Direction
			//D3DXMatrixRotationYawPitchRoll(&matRotY, D3DXToRadian(g_rot_y), 0, D3DXToRadian(g_rot_x));
			//matWorld = matTranslation;

			//D3DXMATRIX matCamTranslation;
			//D3DXMatrixTranslation( &matCamTranslation, -m_cam_pos.x, -m_cam_pos.y, -m_cam_pos.z ); 

			//m_arc_ball.get_matrix()

			//matView = m_arc_ball.get_matrix() * matCamTranslation *  matView;//(matRotY/* * matRotX*/) * matCamTranslation * matView;

			D3DXMATRIX matWorld;
			D3DXMatrixIdentity(&matWorld);

			m_device.get_dx_device()->SetTransform( D3DTS_VIEW, &m_camera.getViewMatrix() );
			m_device.get_dx_device()->SetTransform( D3DTS_WORLD, &matWorld );	// Set The Transformation

			m_device.begin();
			//m_device.clear(Grey);
			m_device.clear(0x00595979);
			m_device.get_dx_device()->SetStreamSource( 0, pVertexBuffer, 0, sizeof(tVertex) );
			m_device.get_dx_device()->SetFVF( D3DFVF_CUSTOMVERTEX );

			m_device.get_dx_device()->DrawPrimitive( D3DPT_TRIANGLESTRIP,  0, 2 ); // Draw Front
			m_device.get_dx_device()->DrawPrimitive( D3DPT_TRIANGLESTRIP,  4, 2 ); // Draw Back
			m_device.get_dx_device()->DrawPrimitive( D3DPT_TRIANGLESTRIP,  8, 2 ); // Draw Top
			m_device.get_dx_device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 12, 2 ); // Draw Bottom
			m_device.get_dx_device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 16, 2 ); // Draw Right
			m_device.get_dx_device()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 20, 2 ); // Draw Left

			m_font.render(L"Hello", rect(10,10,100,100),Red, true);
			m_device.end();
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
			//m_arc_ball.click(xPos, yPos);
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
			// LoWord=Width, HiWord=Height
			resize_scene(LOWORD(lparam), HIWORD(lparam));
			return 0;
		}
	case WM_MOUSEWHEEL:
		{
			float delta = (short)HIWORD((DWORD)wparam);//120.0f;
			delta /= 80.0f;
			m_camera.goForward(-delta);
			//m_cam_pos += D3DXVECTOR3(0.01f,0,0)*delta;
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

				//g_rot_y -= dx/10.0f;
				//g_rot_x += dy/10.0f;
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
	m_device.get_dx_device()->SetTransform( D3DTS_PROJECTION, &m_camera.getProjMatrix()/*D3DMATRIX)projection_matrix */);
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
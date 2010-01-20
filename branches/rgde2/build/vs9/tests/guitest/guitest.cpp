//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Empty starting point for new Direct3D 9 applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "resource.h"

#include "UITest.h"
#include "Xml2Test.h"

#include "guicon.h"

using namespace gui;

UITest* g_test;
float	g_width;
float	g_height;
//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	pDeviceSettings->d3d9.pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	if(!g_test)
	{
		g_test = new Xml2Test;
		g_test->createGUISystem(pd3dDevice);
	}

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{	
    return g_test->OnResetDevice();
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	if(g_test && !g_test->isFinished())
		g_test->Update(fElapsedTime);
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 127, 127, 127), 0.1f, 0) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		if(g_test)
			g_test->Render();
        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
	if(g_test && g_test->isInitialized())
	{
		switch (uMsg) 
		{
			case WM_MOUSEMOVE:
				g_test->handleMouseMove(LOWORD(lParam), HIWORD(lParam));				
				break;

			case WM_CHAR:
				g_test->handleChar((UINT_PTR)wParam);
				break;

			case WM_KEYDOWN:
				g_test->handleKeyboard((UINT_PTR)wParam, EventArgs::Down);
				break;

			case WM_KEYUP:
				g_test->handleKeyboard((UINT_PTR)wParam, EventArgs::Up);
				break;

			case WM_LBUTTONDOWN:
				g_test->handleMouseButton(EventArgs::Left, EventArgs::Down);
				break;

			case WM_LBUTTONUP:
				g_test->handleMouseButton(EventArgs::Left, EventArgs::Up);
				break;

			case WM_RBUTTONDOWN:
				g_test->handleMouseButton(EventArgs::Right, EventArgs::Down);
				break;

			case WM_RBUTTONUP:
				g_test->handleMouseButton(EventArgs::Right, EventArgs::Up);
				break;

			case WM_MBUTTONDOWN:
				g_test->handleMouseButton(EventArgs::Middle, EventArgs::Down);				
				break;

			case WM_MBUTTONUP:
				g_test->handleMouseButton(EventArgs::Middle, EventArgs::Up);
				break;

			case WM_MOUSEWHEEL:
				int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				g_test->handleMouseWheel(delta);
				break;
		}

	}
    return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	if(g_test)
		g_test->OnLostDevice();
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	if(g_test)
	{
		delete 	g_test;
		g_test = NULL;
	}
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <boost/filesystem.hpp>

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR params, int )
{
	wchar_t buf[512];
	GetModuleFileNameW(NULL, &buf[0], 512);

	boost::filesystem::wpath p(buf);
	std::wstring path = p.branch_path().string() + L"/../data/";
	SetCurrentDirectoryW(path.c_str());


	g_test = NULL;
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions
    DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackFrameMove( OnFrameMove );

	g_width = 1024.f;
	g_height = 768.f;
	std::wstring res(params);
	if(!res.empty())
	{
		size_t delim = res.find_first_of(L" ");
		if(delim != std::wstring::npos)
		{
			g_width = (float)_wtof(res.substr(0, delim).c_str());
			g_height = (float)_wtof(res.substr(delim).c_str());
		}
	}
	RedirectIOToConsole();

    // Initialize DXUT and create the desired Win32 window and Direct3D device for the application
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, false, true );  // handle the default hotkeys
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"GUI Test" );
    DXUTCreateDevice( true, (int)g_width, (int)g_height );

    // Start the render loop
    DXUTMainLoop();
    
    return DXUTGetExitCode();
}



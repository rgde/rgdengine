#include "precompiled.h"

#include "render_system_impl.h"

#include <rgde/base/singelton.h>
#include <rgde/base/xml_helpers.h>
#include "../base/exception.h"

#include <rgde/render/render_device.h>
#include <rgde/render/effect.h>
#include <rgde/render/sprites.h>




#ifdef _DEBUG
	#pragma comment (lib, "dxguid.lib")
	#pragma comment (lib, "d3d9.lib")
	#pragma comment (lib, "d3dx9d.lib")
	#pragma comment (lib, "d3dxof.lib")	
	#pragma comment (lib, "dinput8.lib")
	#pragma comment (lib, "dxerr9.lib" )
#else
	#pragma comment (lib, "dxguid.lib")
	#pragma comment (lib, "d3d9.lib")
	#pragma comment (lib, "d3dx9.lib")
	#pragma comment (lib, "d3dxof.lib")	
	#pragma comment (lib, "dinput8.lib")
	#pragma comment (lib, "dxerr9.lib" )
#endif

LPDIRECT3DDEVICE9       g_d3d							= NULL;
LPDIRECT3DSURFACE9		g_pDefaultColorTarget			= NULL;
LPDIRECT3DSURFACE9		g_pDefaultDepthStencilTarget	= NULL;
D3DVIEWPORT9			g_DefaultViewport;

namespace core
{


device_info::device_info()
{
	m_windowed = true;
	m_depth_buffer_format = D3DFMT_D24S8;
	m_back_buffer_format = D3DFMT_X8R8G8B8;
	m_refresh_rate = 0;
	m_VertexProcessingMode = 0;
	m_sync = false;

	m_clear_color = math::Color(100,100,100,255);

	m_VertexProcessingMode = D3DCREATE_HARDWARE_VERTEXPROCESSING;
}

device_info::device_info(bool bWindowed, D3DFORMAT DepthStencilFormat,
	D3DFORMAT BackBufferFormat, int nRefreshRate,
	const DWORD& VertexProcessingMode,
	const math::Color& ClearColor, bool vsync)
{
	m_windowed            = bWindowed;
	m_depth_buffer_format   = DepthStencilFormat;
	m_back_buffer_format     = BackBufferFormat;
	m_refresh_rate         = nRefreshRate;
	m_VertexProcessingMode = VertexProcessingMode;
	m_clear_color           = ClearColor;
	m_sync = vsync;
}



render_device_impl::render_device_impl(HWND hwnd)
: m_pD3D(NULL),
m_pd3dDevice(NULL),
m_hwnd(hwnd),
m_is_first_frame(true)
{
	subscribe<window_resize>(&render_device_impl::onWindowResizeEvent);
	init(hwnd);
}

// Releases all previously initialized objects
render_device_impl::~render_device_impl()
{
	m_lines2d.reset();
	m_lines3d.reset();

	::render::TheRenderManager::destroy();
	::render::effect::clear_all();

	SAFE_RELEASE(g_pDefaultColorTarget);
	SAFE_RELEASE(g_pDefaultDepthStencilTarget);

	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
}

void render_device_impl::onWindowResizeEvent(window_resize e) 
{
	return;
	render::render_device::on_lost();

	// do some device updates if needed
	m_d3dpp.BackBufferWidth = e.width;
	m_d3dpp.BackBufferHeight = e.height;

	SAFE_RELEASE(g_pDefaultColorTarget);
	SAFE_RELEASE(g_pDefaultDepthStencilTarget);

	m_pd3dDevice->Reset(&m_d3dpp);

	// Setup default render targets
	V(m_pd3dDevice->GetRenderTarget(0, &g_pDefaultColorTarget));
	V(m_pd3dDevice->GetDepthStencilSurface(&g_pDefaultDepthStencilTarget));

	// Resize default viewport
	g_DefaultViewport.X = 0;
	g_DefaultViewport.Y = 0;
	g_DefaultViewport.Width = e.width;
	g_DefaultViewport.Height= e.height;
	g_DefaultViewport.MinZ	= 0.0f;
	g_DefaultViewport.MaxZ	= 1.0f;

	render::render_device::on_reset();
}

void render_device_impl::save_screen(const std::wstring& file_name)
{
	D3DDISPLAYMODE display;
	m_pd3dDevice->GetDisplayMode(0, &display);

	LPDIRECT3DSURFACE9 ScreenShotSurface = NULL;

	// save the back buffer to the surface.  Then save the file. 
	m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &ScreenShotSurface);
	D3DXSaveSurfaceToFile((file_name + L".png").c_str(), D3DXIFF_PNG, ScreenShotSurface, NULL, NULL);

	SAFE_RELEASE(ScreenShotSurface);
}


// Draws the scene
void render_device_impl::update() const
{
	if( NULL == m_pd3dDevice )
		return;

	if (!m_is_first_frame)
	{
		// Present the backbuffer contents to the display
		HRESULT hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
		//if (hr == D3DERR_DEVICELOST) - сделать проверки на потерю девайса и т.д.
	}

	m_is_first_frame = false;

	// Clear the backbuffer
	math::Color color = render::render_device::get_clear_color();
	V(m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), 1.0f, 0 ));

	// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		::render::TheRenderManager::get().renderScene();

		// End the scene
		V(m_pd3dDevice->EndScene());
	}
}

D3DFORMAT render_device_impl::getBackBufferFormat(const std::string& strColorBufferFormat)
{
	if(strColorBufferFormat == "A2R10G10B10")
		return D3DFMT_A2R10G10B10;
	else
		if(strColorBufferFormat == "A8R8G8B8")
			return D3DFMT_A8R8G8B8;
		else
			if(strColorBufferFormat == "X8R8G8B8")
				return D3DFMT_X8R8G8B8;
			else
				if(strColorBufferFormat == "A1R5G5B5")
					return D3DFMT_A1R5G5B5;
				else
					if(strColorBufferFormat == "X1R5G5B5")
						return D3DFMT_X1R5G5B5;
					else
						if(strColorBufferFormat == "R5G6B5")
							return D3DFMT_R5G6B5;

	base::lerr<<"Bad Back Buffer format ("<<strColorBufferFormat<<")";
	return D3DFMT_UNKNOWN;
}

D3DFORMAT render_device_impl::getDepthStencilFormat(int nDepthBits, int nStencilBits)
{
	if(nDepthBits == 24 && nStencilBits == 8)
		return D3DFMT_D24S8;
	else
		if(nDepthBits == 32 && nStencilBits == 0)
			return D3DFMT_D32;
		else
			if(nDepthBits == 15 && nStencilBits == 1)
				return D3DFMT_D15S1;
			else
				if(nDepthBits == 16 && nStencilBits == 0)
					return D3DFMT_D16;
				else
					if(nDepthBits == 24 && nStencilBits == 4)
						return D3DFMT_D24X4S4;

	base::lerr<<"Unsupported Depth Stencil Buffer format ("<<nDepthBits<<", "<<nStencilBits<<")";
	return D3DFMT_UNKNOWN;
}

DWORD render_device_impl::getVertexProcessingMode(const std::string& strVertexProcessingMode)
{
	std::string strVertexProcessingModeUpper = strVertexProcessingMode;
	base::upper_case(strVertexProcessingModeUpper);

	if(strVertexProcessingModeUpper == "SOFTWARE")
		return D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		if(strVertexProcessingModeUpper == "HARDWARE")
			return D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			if(strVertexProcessingModeUpper == "MIXED")
				return D3DCREATE_MIXED_VERTEXPROCESSING;

	base::lerr<<"Bad Vertex Processing Method ("<<strVertexProcessingMode<<")";
	return -1;
}

device_info render_device_impl::readRenderDeviceInfo(const std::string strConfigName)
{
	device_info deviceInfo;

	TiXmlDocument XmlConfig;

	if(!base::load_xml(strConfigName, XmlConfig))
	{
		base::lwrn << "readRenderDeviceParameters(): Can't open config file: \""<<strConfigName<<"\"";
		base::lwrn << "Using default settings";
		return deviceInfo;
	}

	TiXmlHandle hConfigHandle(&XmlConfig);
	TiXmlHandle WindowNode = hConfigHandle.FirstChildElement("Engine").FirstChildElement("Window");
	TiXmlHandle RenderDeviceNode = hConfigHandle.FirstChildElement("Engine").FirstChildElement("RenderDevice");

	//deviceInfo.width = base::safe_read<int>(WindowNode, "width", 640);
	//deviceInfo.height = base::safe_read<int>(WindowNode, "height", 480);

	deviceInfo.m_refresh_rate = base::safe_read<int>(WindowNode, "RefreshRate", 85);
	deviceInfo.m_windowed = !base::safe_read<int>(WindowNode, "Fullscreen", 0);
	deviceInfo.m_sync = 1 == base::safe_read<int>(RenderDeviceNode, "m_sync", 0);

	if(deviceInfo.m_windowed && deviceInfo.m_refresh_rate != 0)
	{
		base::lwrn<<"readRenderDeviceInfo(): RefreshRate isn't 0 for windowed mode. Setting up to 0";
		deviceInfo.m_refresh_rate = 0;
	}

	deviceInfo.m_clear_color.r = base::safe_read_attr<int>(RenderDeviceNode, "BackColor", "r", 100);
	deviceInfo.m_clear_color.g = base::safe_read_attr<int>(RenderDeviceNode, "BackColor", "g", 100);
	deviceInfo.m_clear_color.b = base::safe_read_attr<int>(RenderDeviceNode, "BackColor", "b", 100);
	deviceInfo.m_clear_color.a = base::safe_read_attr<int>(RenderDeviceNode, "BackColor", "a", 255);
	//->
	math::Color color = deviceInfo.m_clear_color;
	render::render_device::set_clear_color(color);
	//-<

	std::string strColorBufferFormat = base::safe_read<std::string>(RenderDeviceNode, "ColorFormat", "A8R8G8B8");

	deviceInfo.m_back_buffer_format = getBackBufferFormat(strColorBufferFormat);

	int nDepthBits = base::safe_read_attr<int>(RenderDeviceNode, "DepthStencilFormat", "DepthBpp", 24);
	int nStencilBits = base::safe_read_attr<int>(RenderDeviceNode, "DepthStencilFormat", "StencilBpp", 8);

	deviceInfo.m_depth_buffer_format = getDepthStencilFormat(nDepthBits, nStencilBits);

	std::string strVertexProcessingMode = base::safe_read<std::string>(RenderDeviceNode, "VertexProcessingMethod", "SOFTWARE");

	deviceInfo.m_VertexProcessingMode = getVertexProcessingMode(strVertexProcessingMode);

	return deviceInfo;
}

// Initializes Direct3D
void render_device_impl::init( void* hHostWindow )
{
	m_hwnd = (HWND)hHostWindow;
	// create the D3D object, which is needed to create the D3DDevice.
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		m_pD3D = NULL;
		return;// E_FAIL;
	}

	device_info deviceInfo = readRenderDeviceInfo("EngineConfig.xml");

	m_clear_color = deviceInfo.m_clear_color;

	if(!initDevice(deviceInfo))
	{
		base::lerr<<"render_device_impl::initDevice(): Can't init D3D render_device";
		exit(1);//is it right?
	}

	render::render_device::init();

	return;

	D3DCAPS9 caps;
	m_pd3dDevice->GetDeviceCaps(&caps);

	for (unsigned i = 0; i < 4; ++i)
	{
		V(g_d3d->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ));
		V(g_d3d->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ));
		V(g_d3d->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR ));
	}

	V(g_d3d->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ));
	V(g_d3d->SetRenderState( D3DRS_LIGHTING, FALSE ));
	V(g_d3d->SetRenderState( D3DRS_ZENABLE, TRUE ));

	// render_device state would normally be set here
	return;// S_OK;
}

bool render_device_impl::initDevice(const device_info& DeviceInfo)
{
	// This object will allow us to set the display mode of the screen.
	D3DDISPLAYMODE DisplayMode;
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode)))
	{
		base::lerr << "Error setting the display mode.";
		return false;
	}

	RECT rc;
	GetClientRect(m_hwnd, &rc);
	unsigned int width = rc.right - rc.left;
	unsigned int height = rc.bottom - rc.top;

	m_info = DeviceInfo;

	D3DPRESENT_PARAMETERS d3dpp=
	{
		width,			// Back Buffer Width
		height,			// Back Buffer Height
		D3DFMT_X8R8G8B8,		// Back Buffer Format (Color Depth)
		1,				// Back Buffer Count (Double Buffer)
		D3DMULTISAMPLE_4_SAMPLES/*D3DMULTISAMPLE_NONE*/,	// No Multi Sample Type
		0,				// No Multi Sample Quality
		D3DSWAPEFFECT_DISCARD,	// Swap effect (Fast)
		(HWND)m_hwnd,	// The Window Handle
		TRUE,		// Windowed or Fullscreen
		TRUE,			// Enable Auto Depth Stencil  
		D3DFMT_D24S8,	// 16Bit Z-Buffer (Depth Buffer)
		0,				// No Flags
		D3DPRESENT_RATE_DEFAULT,   // Default Refresh Rate
		D3DPRESENT_INTERVAL_IMMEDIATE	// Presentation Interval (vertical sync)
	};

	m_d3dpp = d3dpp;

	// Set default settings
	UINT AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

	for (UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
	{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT Res=m_pD3D->GetAdapterIdentifier(Adapter,0,&Identifier);
		if (strcmp(Identifier.Description,"NVIDIA\tNVPerfHUD")==0)
		{
			AdapterToUse = Adapter;
			DeviceType = D3DDEVTYPE_REF;
			break;
		}
	}

	m_pd3dDevice = NULL;

	if(FAILED(m_pD3D->CreateDevice( AdapterToUse, DeviceType, m_hwnd,
		DeviceInfo.m_VertexProcessingMode, //TODO:
		&d3dpp, &m_pd3dDevice ) ) )
	{
		return false;
	}


	// Setup default viewport
	m_pd3dDevice->GetViewport(&g_DefaultViewport);			

	// Setup default render targets
	V(m_pd3dDevice->GetRenderTarget(0, &g_pDefaultColorTarget));
	V(m_pd3dDevice->GetDepthStencilSurface(&g_pDefaultDepthStencilTarget));

	g_d3d = m_pd3dDevice;

	return true;
}


}
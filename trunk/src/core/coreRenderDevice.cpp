#include "precompiled.h"

#include <d3d9.h>
#include <d3dx9tex.h>

#include "core/coreRenderDevice.h"

#include "./base/exception.h"
#include "base/singelton.h"
#include "base/xml_helpers.h"

#include "math/mathTypes3d.h"
#include "core/application.h"

#include "render/renderDevice.h"
#include "render/renderEffect.h"
#include "render/renderLines2d.h"
#include "render/renderLines3d.h"
#include "render/renderSprites.h"


LPDIRECT3DDEVICE9       g_pd3dDevice					= NULL;
LPDIRECT3DSURFACE9		g_pDefaultColorTarget			= NULL;
LPDIRECT3DSURFACE9		g_pDefaultDepthStencilTarget	= NULL;
D3DVIEWPORT9			g_DefaultViewport;

namespace core
{
	struct SRenderDeviceInfo
	{
		SRenderDeviceInfo()
		{
			m_bWindowed = true;
			m_DepthStencilFormat = D3DFMT_UNKNOWN;
			m_BackBufferFormat = D3DFMT_UNKNOWN;
			m_nRefreshRate = 0;
			m_VertexProcessingMode = 0;
			VSync = false;
		}

		SRenderDeviceInfo(bool bWindowed, D3DFORMAT DepthStencilFormat,
							D3DFORMAT BackBufferFormat, int nRefreshRate,
							const DWORD& VertexProcessingMode,
							const math::Color& ClearColor, bool vsync)
		{
			m_bWindowed            = bWindowed;
			m_DepthStencilFormat   = DepthStencilFormat;
			m_BackBufferFormat     = BackBufferFormat;
			m_nRefreshRate         = nRefreshRate;
			m_VertexProcessingMode = VertexProcessingMode;
			m_ClearColor           = ClearColor;
			VSync = vsync;
		}

		bool m_bWindowed;
		D3DFORMAT m_DepthStencilFormat;
		D3DFORMAT m_BackBufferFormat;
		int m_nRefreshRate;
		DWORD m_VertexProcessingMode;
		math::Color m_ClearColor;
		int width;
		int height;
		bool VSync;
	};

	class CDXRenderDevice : public IRenderSystem, public event::CSender
	{
	public:
		CDXRenderDevice(HWND hwnd)
			: 	m_pD3D(NULL),
				m_pd3dDevice(NULL),
				m_hWnd(hwnd)
		{
			subscribe<CWindowResize>(&CDXRenderDevice::onWindowResizeEvent);
			init(hwnd);
		}

		// Releases all previously initialized objects
		virtual ~CDXRenderDevice()
		{
			::render::TheDevice::Destroy(); // here onLost calling
			::render::TheLine2dManager::Destroy();
			::render::Line3dManager::Destroy();
			//::render::TheSpriteManager::Destroy();
			::render::TheRenderManager::Destroy();

			::render::IEffect::ClearAll();

			SAFE_RELEASE(g_pDefaultColorTarget);
			SAFE_RELEASE(g_pDefaultDepthStencilTarget);

			SAFE_RELEASE(m_pd3dDevice);
			SAFE_RELEASE(m_pD3D);

			//base::lmsg << "~CDXRenderDevice()";
		}

		void onWindowResizeEvent(CWindowResize e) 
		{
			return;
			::render::TheDevice::Get().onLost();

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

			::render::TheDevice::Get().onReset();
		}

		virtual void saveScreenShot(const std::wstring& strFileName)
		{
			D3DDISPLAYMODE display;
			m_pd3dDevice->GetDisplayMode(0, &display);

			LPDIRECT3DSURFACE9 ScreenShotSurface = NULL;

			// Save the back buffer to the surface.  Then save the file. 
			m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &ScreenShotSurface);
			D3DXSaveSurfaceToFile((strFileName + L".png").c_str(), D3DXIFF_PNG, ScreenShotSurface, NULL, NULL);

			SAFE_RELEASE(ScreenShotSurface);
		}


		// Draws the scene
		virtual void update() const
		{
			if( NULL == m_pd3dDevice )
			    return;

			HRESULT hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

			if (g_pDefaultColorTarget)
				V(g_pd3dDevice->SetRenderTarget(0, g_pDefaultColorTarget));	// restore backbuffer as target
			if (g_pDefaultDepthStencilTarget)
				V(g_pd3dDevice->SetDepthStencilSurface(g_pDefaultDepthStencilTarget));	// else restore default depth-stencil as target
			
			// set default viewport
			V(g_pd3dDevice->SetViewport(&g_DefaultViewport));

			// Clear the backbuffer to a blue color
            //->
            math::Color color = render::TheDevice::Get().getClearColor();
            V(m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), 1.0f, 0 ));
			//V(m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(m_ClearColor.a, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b), 1.0f, 0 ));
            //-<
			
			// Begin the scene
			if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
			{
				::render::TheRenderManager::Get().renderScene();
			
			    // End the scene
			    V(m_pd3dDevice->EndScene());
			}
		
			// Present the backbuffer contents to the display
			//HRESULT hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
			//if (hr == D3DERR_DEVICELOST) - ������� �������� �� ������ ������� � �.�.

		}

		inline D3DFORMAT getBackBufferFormat(const std::string& strColorBufferFormat)
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
			exit(1);
		}

		inline D3DFORMAT getDepthStencilFormat(int nDepthBits, int nStencilBits)
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
			exit(1);
		}

		inline DWORD getVertexProcessingMode(const std::string& strVertexProcessingMode)
		{
			std::string strVertexProcessingModeUpper = strVertexProcessingMode;
			base::Upper_case(strVertexProcessingModeUpper);

			if(strVertexProcessingModeUpper == "SOFTWARE")
				return D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			else
			if(strVertexProcessingModeUpper == "HARDWARE")
				return D3DCREATE_HARDWARE_VERTEXPROCESSING;
			else
			if(strVertexProcessingModeUpper == "MIXED")
				return D3DCREATE_MIXED_VERTEXPROCESSING;

			base::lerr<<"Bad Vertex Processing Method ("<<strVertexProcessingMode<<")";
			exit(1);
		}

		inline SRenderDeviceInfo readRenderDeviceInfo(const std::string strConfigName)
		{
			SRenderDeviceInfo deviceInfo;

			TiXmlDocument XmlConfig;

			if(!base::loadXml(strConfigName, XmlConfig))
			{
				base::lerr << "readRenderDeviceParameters(): Can't open config file: \""<<strConfigName<<"\".";
				exit(1);
			}

			TiXmlHandle hConfigHandle(&XmlConfig);
			TiXmlHandle WindowNode = hConfigHandle.FirstChildElement("Engine").FirstChildElement("Window");
			TiXmlHandle RenderDeviceNode = hConfigHandle.FirstChildElement("Engine").FirstChildElement("RenderDevice");

			deviceInfo.width = base::safeReadValue<int>(WindowNode, "width", 640);
			deviceInfo.height = base::safeReadValue<int>(WindowNode, "height", 480);

			deviceInfo.m_nRefreshRate = base::safeReadValue<int>(WindowNode, "RefreshRate", 85);
			deviceInfo.m_bWindowed = !base::safeReadValue<int>(WindowNode, "Fullscreen", 0);
			deviceInfo.VSync = 1 == base::safeReadValue<int>(RenderDeviceNode, "VSync", 0);

			if(deviceInfo.m_bWindowed && deviceInfo.m_nRefreshRate != 0)
			{
				base::lwrn<<"readRenderDeviceInfo(): RefreshRate isn't 0 for windowed mode. Setting up to 0";
				deviceInfo.m_nRefreshRate = 0;
			}

			deviceInfo.m_ClearColor.r = base::safeReadAttributeValue<int>(RenderDeviceNode, "BackColor", "r", 100);
			deviceInfo.m_ClearColor.g = base::safeReadAttributeValue<int>(RenderDeviceNode, "BackColor", "g", 100);
			deviceInfo.m_ClearColor.b = base::safeReadAttributeValue<int>(RenderDeviceNode, "BackColor", "b", 100);
			deviceInfo.m_ClearColor.a = base::safeReadAttributeValue<int>(RenderDeviceNode, "BackColor", "a", 255);
            //->
            math::Color color = deviceInfo.m_ClearColor;
            render::TheDevice::Get().setClearColor(color);
            //-<

			std::string strColorBufferFormat = base::safeReadValue<std::string>(RenderDeviceNode, "ColorFormat", "A8R8G8B8");

			deviceInfo.m_BackBufferFormat = getBackBufferFormat(strColorBufferFormat);

			int nDepthBits = base::safeReadAttributeValue<int>(RenderDeviceNode, "DepthStencilFormat", "DepthBpp", 24);
			int nStencilBits = base::safeReadAttributeValue<int>(RenderDeviceNode, "DepthStencilFormat", "StencilBpp", 8);

			deviceInfo.m_DepthStencilFormat = getDepthStencilFormat(nDepthBits, nStencilBits);

			std::string strVertexProcessingMode = base::safeReadValue<std::string>(RenderDeviceNode, "VertexProcessingMethod", "SOFTWARE");

			deviceInfo.m_VertexProcessingMode = getVertexProcessingMode(strVertexProcessingMode);

			return deviceInfo;
		}

		// Initializes Direct3D
		virtual void init( void* hHostWindow )
		{
			m_hWnd = (HWND)hHostWindow;
			// Create the D3D object, which is needed to create the D3DDevice.
			if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
			{
				m_pD3D = NULL;
			    return;// E_FAIL;
			}

			SRenderDeviceInfo deviceInfo = readRenderDeviceInfo("EngineConfig.xml");

			m_ClearColor = deviceInfo.m_ClearColor;

			if(!initDevice(deviceInfo))
			{
				base::lerr<<"CDXRenderDevice::initDevice(): Can't init D3D Device";
				exit(1);//is it right?
			}

			D3DCAPS9 caps;
			m_pd3dDevice->GetDeviceCaps(&caps);

			for (unsigned i = 0; i < 4; ++i)
			{
				V(g_pd3dDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ));
				V(g_pd3dDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ));
				V(g_pd3dDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR ));
			}

			V(g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ));
			V(g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ));
			V(g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ));
		
			// Device state would normally be set here
			return;// S_OK;
		}

	private:
		inline bool initDevice(const SRenderDeviceInfo& DeviceInfo)
		{
			// This object will allow us to set the display mode of the screen.
			D3DDISPLAYMODE DisplayMode;
			if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode)))
			{
				base::lerr << "Error setting the display mode.";
				return false;
			}

			m_info = DeviceInfo;
			D3DPRESENT_PARAMETERS d3dpp; 
			ZeroMemory( &d3dpp, sizeof(d3dpp) );

			if(!DeviceInfo.m_bWindowed)
			{
				d3dpp.Windowed = FALSE;               // Window mode (fullscreen).
				d3dpp.BackBufferWidth = DeviceInfo.width;
				d3dpp.BackBufferHeight = DeviceInfo.height;
				d3dpp.FullScreen_RefreshRateInHz = DeviceInfo.m_nRefreshRate;
			}
			else
				d3dpp.Windowed = TRUE; 
			
			if (DeviceInfo.VSync)
				d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //SYNC ON
			else
				d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//VSYNC OFF		

			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = DeviceInfo.m_DepthStencilFormat;
			d3dpp.BackBufferFormat = DisplayMode.Format;//DeviceInfo.m_BackBufferFormat;
			d3dpp.hDeviceWindow = m_hWnd;

			m_d3dpp = d3dpp;

			//RECT rc;
			//GetWindowRect(m_hWnd, &rc);

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

			if(FAILED(m_pD3D->CreateDevice( AdapterToUse, DeviceType, m_hWnd,
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
			
			g_pd3dDevice = m_pd3dDevice;

			return true;
		}

	protected:
		D3DPRESENT_PARAMETERS	m_d3dpp;
		HWND					m_hWnd;
		LPDIRECT3D9             m_pD3D;			// Used to create the D3DDevice
		LPDIRECT3DDEVICE9       m_pd3dDevice;	// Our rendering device
		math::Color             m_ClearColor;   // Back Buffer clear color
		SRenderDeviceInfo		m_info;
	};

	PRenderSystem IRenderSystem::Create(const IApplication& app)
	{
		return PRenderSystem(new CDXRenderDevice((HWND)app.getWindowHandle()));
	}
}
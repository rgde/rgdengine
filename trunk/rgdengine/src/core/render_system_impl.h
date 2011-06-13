#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>

#include <rgde/math/types3d.h>
#include <rgde/core/application.h>
#include <rgde/core/render_system.h>
#include <rgde/render/render_device.h>

namespace core
{

struct device_info
{
	device_info();

	device_info(bool bWindowed, D3DFORMAT DepthStencilFormat,
		D3DFORMAT BackBufferFormat, int nRefreshRate,
		const DWORD& VertexProcessingMode,
		const math::Color& ClearColor, bool vsync);

	bool m_windowed;
	D3DFORMAT m_depth_buffer_format;
	D3DFORMAT m_back_buffer_format;
	int m_refresh_rate;
	DWORD m_VertexProcessingMode;
	math::Color m_clear_color;
	bool m_sync;
};


class render_device_impl : public event::sender, 
						   public event::listener,
						   public render::render_device
{	
public:
	render_device_impl(HWND hwnd);
	~render_device_impl();

	void onWindowResizeEvent(window_resize e);
	void save_screen(const std::wstring& filename);
	void update() const;

	D3DFORMAT getBackBufferFormat(const std::string& strColorBufferFormat);

	D3DFORMAT getDepthStencilFormat(int nDepthBits, int nStencilBits);

	DWORD getVertexProcessingMode(const std::string& strVertexProcessingMode);

	device_info readRenderDeviceInfo(const std::string strConfigName);

	// Initializes Direct3D
	virtual void init( void* hHostWindow );

	virtual render::render_device& get_device() {return *this;}
	virtual const render::render_device& get_device() const {return *this;}

private:
	bool initDevice(const device_info& DeviceInfo);

private:
	D3DPRESENT_PARAMETERS	m_d3dpp;
	HWND					m_hwnd;
	LPDIRECT3D9             m_pD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;	// Our rendering device
	math::Color             m_clear_color;   // Back Buffer clear color
	
	device_info				m_info;

	mutable bool			m_is_first_frame;
};

}
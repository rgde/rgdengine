#include "precompiled.h"

#include <rgde/render/camera.h>
#include <rgde/render/render_target.h>

#include <d3dx9.h>

extern LPDIRECT3DDEVICE9   g_pd3dDevice;
extern 	LPDIRECT3DSURFACE9 g_pDefaultColorTarget;
extern 	LPDIRECT3DSURFACE9 g_pDefaultDepthStencilTarget;
extern  D3DVIEWPORT9	   g_DefaultViewport;

#include "../base/exception.h"

namespace render
{
	render_camera::render_camera(unsigned priority, const viewport& viewport)
		: m_priority(priority),
		  m_viewport(viewport),
		  m_clear_color(0),
		  m_clear_depth(1.0f)
	{
		//TheCameraManager::get().addCamera(this);
	}

	render_camera::~render_camera()
	{
		int i = 5;
		//TheCameraManager::get().removeCamera(this);
	}

	void render_camera::activate()
	{
		D3DVIEWPORT9	viewport;
		DWORD			clearFlags = 0;

		viewport.X		= (DWORD) m_viewport.x;
		viewport.Y		= (DWORD) m_viewport.y;
		viewport.Width	= (DWORD) m_viewport.w;
		viewport.Height = (DWORD) m_viewport.h;
		viewport.MinZ	= 0.0f;
		viewport.MaxZ	= 1.0f;

		// !!! here we need to check viewport for non zero dimentions !!!
		g_pd3dDevice->SetViewport(&viewport);

		// If we already save backbuffer surface pointer
		if (g_pDefaultColorTarget)
		{
			// If we have color target
			if (m_pColorTarget)
			{
				m_pColorTarget->activate();	// set it

				// Set color clear flag
				clearFlags |= D3DCLEAR_TARGET;
			}
			else
			{
				V(g_pd3dDevice->SetRenderTarget(0, g_pDefaultColorTarget));	// else restore backbuffer as target
			}
		}
		else
		{
			// if we will change color target
			if (m_pColorTarget)
			{
				// save backbuffer surface pointer
				V(g_pd3dDevice->GetRenderTarget(0, &g_pDefaultColorTarget));

				// set new color target
				m_pColorTarget->activate();

				// Set color clear flag
				clearFlags |= D3DCLEAR_TARGET;
			}
		}

		// If we already save default depth-stencil surface pointer
		if (g_pDefaultDepthStencilTarget)
		{
			// If we have depth-stencil target
			if (m_pDepthStencilTarget)
			{
				m_pDepthStencilTarget->activate();	// set it

				// Set color clear flag
				clearFlags |= D3DCLEAR_ZBUFFER;
			}
			else
			{
				V(g_pd3dDevice->SetDepthStencilSurface(g_pDefaultDepthStencilTarget));	// else restore default depth-stencil as target
			}
		}
		else
		{
			// if we will change depth-stencil target
			if (m_pDepthStencilTarget)
			{
				// save default depth-stencil surface pointer
				V(g_pd3dDevice->GetDepthStencilSurface(&g_pDefaultDepthStencilTarget));

				// set new depth-stencil target
				m_pDepthStencilTarget->activate();

				// Set color clear flag
				clearFlags |= D3DCLEAR_ZBUFFER;
			}
		}

		if (clearFlags)
			V(g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_clear_color.color, m_clear_depth, 0));
	
		BaseCamera::activate();
	}

	void render_camera::setDepthStencilTarget(const PRenderTexture& pTarget, float clearDepth)
	{
		m_pDepthStencilTarget	= pTarget;
		m_clear_depth			= clearDepth;
	}

	void render_camera::setColorTarget(const PRenderTexture& pTarget, const math::Color&  clearColor)
	{
		m_pColorTarget	= pTarget;
		m_clear_color	= clearColor;
	}
		
	void render_camera::setPriority(unsigned priority)
	{
		m_priority = priority;
	}

	unsigned render_camera::get_priority() const
	{
		return m_priority;
	}

	camera_ptr render_camera::create()
	{
		return create(0, viewport(g_DefaultViewport.X, g_DefaultViewport.Y, g_DefaultViewport.Width, g_DefaultViewport.Height));
	}

	camera_ptr render_camera::create(unsigned priority, const viewport& viewport)
	{
		return camera_ptr(new render_camera(priority, viewport));
	}
}
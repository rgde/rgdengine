#include "precompiled.h"

#include <rgde/render/camera.h>
#include <rgde/render/render_target.h>
#include <rgde/render/camera_manager.h>

#include <d3dx9.h>

extern LPDIRECT3DDEVICE9   g_d3d;
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
		TheCameraManager::get().add(this);
	}

	render_camera::~render_camera()
	{
		TheCameraManager::get().remove(this);
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
		g_d3d->SetViewport(&viewport);

		// If we already save backbuffer surface pointer
		if (g_pDefaultColorTarget)
		{
			// If we have color target
			if (m_color_target)
			{
				m_color_target->activate();	// set it

				// Set color clear flag
				clearFlags |= D3DCLEAR_TARGET;
			}
			else
			{
				V(g_d3d->SetRenderTarget(0, g_pDefaultColorTarget));	// else restore backbuffer as target
			}
		}
		else
		{
			// if we will change color target
			if (m_color_target)
			{
				// save backbuffer surface pointer
				V(g_d3d->GetRenderTarget(0, &g_pDefaultColorTarget));

				// set new color target
				m_color_target->activate();

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
				V(g_d3d->SetDepthStencilSurface(g_pDefaultDepthStencilTarget));	// else restore default depth-stencil as target
			}
		}
		else
		{
			// if we will change depth-stencil target
			if (m_pDepthStencilTarget)
			{
				// save default depth-stencil surface pointer
				V(g_d3d->GetDepthStencilSurface(&g_pDefaultDepthStencilTarget));

				// set new depth-stencil target
				m_pDepthStencilTarget->activate();

				// Set color clear flag
				clearFlags |= D3DCLEAR_ZBUFFER;
			}
		}

		if (clearFlags)
			V(g_d3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_clear_color.color, m_clear_depth, 0));
	
		base_camera::activate();
	}

	void render_camera::setDepthStencilTarget(const render_texture_ptr& pTarget, float clearDepth)
	{
		m_pDepthStencilTarget	= pTarget;
		m_clear_depth			= clearDepth;
	}

	void render_camera::setColorTarget(const render_texture_ptr& pTarget, const math::Color&  clearColor)
	{
		m_color_target	= pTarget;
		m_clear_color	= clearColor;
	}
		
	void render_camera::priority(unsigned priority)
	{
		m_priority = priority;
	}

	unsigned render_camera::priority() const
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
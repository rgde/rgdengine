#include "precompiled.h"

#include "render/renderCamera.h"
#include "render/renderRenderTarget.h"

#include <d3dx9.h>

extern LPDIRECT3DDEVICE9   g_pd3dDevice;
extern 	LPDIRECT3DSURFACE9 g_pDefaultColorTarget;
extern 	LPDIRECT3DSURFACE9 g_pDefaultDepthStencilTarget;
extern  D3DVIEWPORT9	   g_DefaultViewport;

#include "../base/exception.h"

namespace render
{
	CRenderCamera::CRenderCamera(unsigned priority, const SViewPort& viewport)
		: m_Priority(priority),
		  m_Viewport(viewport),
		  m_ClearColor(0),
		  m_ClearDepth(1.0f)
	{
		//TheCameraManager::Get().addCamera(this);
	}

	CRenderCamera::~CRenderCamera()
	{
		int i = 5;
		//TheCameraManager::Get().removeCamera(this);
	}

	void CRenderCamera::activate()
	{
		D3DVIEWPORT9	viewport;
		DWORD			clearFlags = 0;

		viewport.X		= (DWORD) m_Viewport.x;
		viewport.Y		= (DWORD) m_Viewport.y;
		viewport.Width	= (DWORD) m_Viewport.w;
		viewport.Height = (DWORD) m_Viewport.h;
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
			V(g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_ClearColor.color, m_ClearDepth, 0));
	
		CBaseCamera::activate();
	}

	void CRenderCamera::setDepthStencilTarget(const PRenderTexture& pTarget, float clearDepth)
	{
		m_pDepthStencilTarget	= pTarget;
		m_ClearDepth			= clearDepth;
	}

	void CRenderCamera::setColorTarget(const PRenderTexture& pTarget, const math::Color&  clearColor)
	{
		m_pColorTarget	= pTarget;
		m_ClearColor	= clearColor;
	}
		
	void CRenderCamera::setPriority(unsigned priority)
	{
		m_Priority = priority;
	}

	unsigned CRenderCamera::getPriority() const
	{
		return m_Priority;
	}

	PCamera CRenderCamera::Create()
	{
		return Create(0, SViewPort(g_DefaultViewport.X, g_DefaultViewport.Y, g_DefaultViewport.Width, g_DefaultViewport.Height));
	}

	PCamera CRenderCamera::Create(unsigned priority, const SViewPort& viewport)
	{
		return PCamera(new CRenderCamera(priority, viewport));
	}
}
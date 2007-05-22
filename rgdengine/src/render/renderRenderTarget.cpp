#include "precompiled.h"

#include <rgde/render/renderRenderTarget.h>

#include "renderTextureImpl.h"

#include "../base/exception.h"

extern IDirect3DDevice9* g_pd3dDevice;

namespace render
{
	class RenderTextureImpl : public IRenderTexture, public CTextureImpl
	{
	public:
		TextureFormat getFormat() const {return CTextureImpl::getFormat();}
		TextureUsage  getUsage()  const {return CTextureImpl::getUsage();}
		int			  getHeight() const {return CTextureImpl::getHeight();}
		int			  getWidth()  const {return CTextureImpl::getWidth();}
		ETextureType  getType()	  const {return CTextureImpl::getType();}
		bool		  hasAlpha()  const {return CTextureImpl::hasAlpha();}

		RenderTextureImpl(const SRenderTargetInfo& params)
		{
			createRenderTexture(params.size, params.format);
		}

		~RenderTextureImpl()
		{
		}

		void activate()
		{
			IDirect3DSurface9 *pSurfaceLevel = NULL;

			V(m_pTexture->GetSurfaceLevel(0, &pSurfaceLevel));

			if (DepthStencil == CTextureImpl::getUsage()){
				V(g_pd3dDevice->SetDepthStencilSurface(pSurfaceLevel));
			} else {
				V(g_pd3dDevice->SetRenderTarget(0, pSurfaceLevel));
			}
		}
	};

	PRenderTexture IRenderTexture::Create(const SRenderTargetInfo& params)
	{
		return PRenderTexture(new RenderTextureImpl(params));
	}
}
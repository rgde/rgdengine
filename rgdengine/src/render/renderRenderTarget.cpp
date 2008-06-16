#include "precompiled.h"

#include <rgde/render/renderRenderTarget.h>

#include "renderTextureImpl.h"

#include "../base/exception.h"

extern IDirect3DDevice9* g_pd3dDevice;

namespace render
{
	class RenderTextureImpl : public IRenderTexture, public TextureImpl
	{
	public:
		TextureFormat getFormat() const {return TextureImpl::getFormat();}
		TextureUsage  getUsage()  const {return TextureImpl::getUsage();}
		int			  getHeight() const {return TextureImpl::getHeight();}
		int			  getWidth()  const {return TextureImpl::getWidth();}
		ETextureType  getType()	  const {return TextureImpl::getType();}
		bool		  hasAlpha()  const {return TextureImpl::hasAlpha();}

		RenderTextureImpl(const RenderTargetInfo& params)
		{
			createRenderTexture(params.size, params.format);
		}

		~RenderTextureImpl()
		{
		}

		void activate()
		{
			IDirect3DSurface9 *pSurfaceLevel = NULL;

			V(m_texture->GetSurfaceLevel(0, &pSurfaceLevel));

			if (DepthStencil == TextureImpl::getUsage()){
				V(g_pd3dDevice->SetDepthStencilSurface(pSurfaceLevel));
			} else {
				V(g_pd3dDevice->SetRenderTarget(0, pSurfaceLevel));
			}
		}
	};

	PRenderTexture IRenderTexture::Create(const RenderTargetInfo& params)
	{
		return PRenderTexture(new RenderTextureImpl(params));
	}
}
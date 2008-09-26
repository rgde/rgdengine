#include "precompiled.h"

#include <rgde/render/render_target.h>

#include "texture_impl.h"

#include "../base/exception.h"

extern IDirect3DDevice9* g_pd3dDevice;

namespace render
{
	class render_texture_d3d9 : public render_texture, public texture_d3d9
	{
	public:
		TextureFormat getFormat() const {return texture_d3d9::getFormat();}
		TextureUsage  getUsage()  const {return texture_d3d9::getUsage();}
		int			  getHeight() const {return texture_d3d9::getHeight();}
		int			  getWidth()  const {return texture_d3d9::getWidth();}
		ETextureType  getType()	  const {return texture_d3d9::getType();}
		bool		  hasAlpha()  const {return texture_d3d9::hasAlpha();}

		render_texture_d3d9(const RenderTargetInfo& params)
		{
			createRenderTexture(params.size, params.format);
		}

		~render_texture_d3d9()
		{
		}

		void activate()
		{
			IDirect3DSurface9 *pSurfaceLevel = NULL;

			V(m_texture->GetSurfaceLevel(0, &pSurfaceLevel));

			if (DepthStencil == texture_d3d9::getUsage()){
				V(g_pd3dDevice->SetDepthStencilSurface(pSurfaceLevel));
			} else {
				V(g_pd3dDevice->SetRenderTarget(0, pSurfaceLevel));
			}
		}
	};

	PRenderTexture render_texture::create(const RenderTargetInfo& params)
	{
		return PRenderTexture(new render_texture_d3d9(params));
	}
}
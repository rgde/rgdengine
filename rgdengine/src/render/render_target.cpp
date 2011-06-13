#include "precompiled.h"

#include <rgde/render/render_target.h>

#include "texture_impl.h"

#include "../base/exception.h"

extern IDirect3DDevice9* g_d3d;

namespace render
{
	class render_texture_d3d9 : public render_texture, public texture_d3d9
	{
	public:
		texture_format get_format() const {return texture_d3d9::get_format();}
		texture_usage  get_usage()  const {return texture_d3d9::get_usage();}
		int			  get_height() const {return texture_d3d9::get_height();}
		int			  width()  const {return texture_d3d9::width();}
		texture_type  type()	  const {return texture_d3d9::type();}
		bool		  has_alpha()  const {return texture_d3d9::has_alpha();}

		virtual const std::string& get_filename() const 
		{
			static std::string empty_string;
			return empty_string;
		}

		render_texture_d3d9(const render_target_info& params)
		{
			create_render_texture(params.size, params.format);
		}

		~render_texture_d3d9()
		{
		}

		void activate()
		{
			IDirect3DSurface9 *pSurfaceLevel = NULL;

			V(m_texture->GetSurfaceLevel(0, &pSurfaceLevel));

			if (DepthStencil == texture_d3d9::get_usage())
			{
				V(g_d3d->SetDepthStencilSurface(pSurfaceLevel));
			} else 
			{
				V(g_d3d->SetRenderTarget(0, pSurfaceLevel));
			}
		}
	};

	render_texture_ptr render_texture::create(const render_target_info& params)
	{
		return render_texture_ptr(new render_texture_d3d9(params));
	}
}
#include "stdafx.h"
#include "render_target.h"

#include <rgde/render/surface.h>

namespace rgde
{
	namespace render
	{
		rt_desc::rt_desc(  int width, int height, resource::format texture_format
			, resource::format depth_stencil_format, int screen_divisor
			, size_t size_shift, bool force_crop 
			, bool use_depth_wh, int depth_width
			, int depth_height, bool create_rt_texture
			, bool create_ds_texture)
			: width_(width)
			, height_(height)
			, texture_format_(texture_format)
			, depth_stencil_format_(depth_stencil_format)
			, screen_divisor_(screen_divisor)
			, size_shift_(size_shift)
			, force_crop_(force_crop)
			, use_depth_wh_ (use_depth_wh)
			, depth_width_(depth_width)
			, depth_height_(depth_height)
			, create_rt_texture_(create_rt_texture)
			, create_ds_texture_(create_ds_texture)
		{
			if( !( screen_divisor_ == 0
				|| screen_divisor_ == 1
				|| screen_divisor_ == 2
				|| screen_divisor_ == 4
				|| screen_divisor_ == 8
				) )
			{
				//warning("[render_target::desc::desc] Invalid screen_divisor_ %d. Reset to full screen.", screen_divisor_);
				screen_divisor_ = 1;				
			}
		}

		bool rt_desc::operator == (const rt_desc& rhs) const
		{
			return	(   width_ == rhs.width_ 
				&&  height_ == rhs.height_ 
				&&  texture_format_ == rhs.texture_format_ 
				&&  depth_stencil_format_ == rhs.depth_stencil_format_ 
				&&  screen_divisor_ == rhs.screen_divisor_ 
				&&  size_shift_ == rhs.size_shift_ 
				&&  force_crop_ == rhs.force_crop_ 
				&&  use_depth_wh_ == rhs.use_depth_wh_ 
				&&  depth_width_ == rhs.depth_width_ 
				&&  depth_height_ == rhs.depth_height_ 
				&&  create_rt_texture_ == rhs.create_rt_texture_ 
				&&  create_ds_texture_ == rhs.create_ds_texture_
				);
		}

		//////////////////////////////////////////////////////////////////////////
		render_target::render_target(device& device)
			: m_locked(false)
			, device_object(device)
		{ 
		}

		render_target::render_target(device& device, surface_ptr rt, surface_ptr depth)
			: m_locked(false)
			, device_object(device)
			, m_surface(rt)
			, m_depth_surface(depth)
		{
		}

		render_target::~render_target()
		{ 
			release(); 
		}

		void render_target::release()
		{
			m_texture.reset();
			m_surface.reset();
			m_depth_surface.reset();
			m_depth_texture.reset();
			m_locked = false;
		}

		const rt_desc& render_target::get_desc() const
		{ 
			return m_desc; 
		}

		bool render_target::operator== (const render_target& rhs) const 
		{ 
			return m_desc == rhs.get_desc(); 
		}

		void render_target::clear_texture( unsigned long argb_color )
		{
			if( m_surface )
			{
				//if( m_device.color_fill( m_surface, argb_color ) )
				{
					//LogPrintf( "render_target::clear_texture ColorFill failed: %s", DXGetErrorDescription9A(hr) );
				}
			}
		}

		void render_target::set_current()
		{
			if( m_surface )
			{
				if( !get_device().set_render_target(0, m_surface) )
				{
					throw std::exception( "render_target::set_current SetRenderTarget: failed. " );
				}
			}

			if( m_depth_surface )
			{
				if( !get_device().set_depth_surface( m_depth_surface ) )
				{
					throw std::exception( "render_target::set_current SetDepthStencilSurface: failed. " );
				}
			}
		}

		void render_target::on_device_lost()
		{
			release();
		}

		void render_target::on_device_reset()
		{
			view_port vp = get_device().get_viewport();

			int width = (int)vp.width;
			int height = (int)vp.height;

			if( m_desc.screen_divisor_ == 0 )
			{
				//<do not change the size./>
			}
			else
				if( m_desc.screen_divisor_ == 1 && !m_desc.force_crop_ )
				{
					m_desc.width_ = width;
					m_desc.height_ = height;
				}
				else
					if( m_desc.screen_divisor_ <= 8 )
					{
						int crop_width = width - width % 8;
						int crop_height = height - height % 8;

						m_desc.width_ = crop_width/m_desc.screen_divisor_ + (int)m_desc.size_shift_;
						m_desc.height_ = crop_height/m_desc.screen_divisor_ + (int)m_desc.size_shift_;
					}

					reinit();
		}

		void render_target::reinit()								
		{ 
			if(m_desc.texture_format_)
			{
				if (m_desc.create_rt_texture_)
				{
					m_texture = texture::create(get_device(), m_desc.width_, m_desc.height_, 1,
						m_desc.texture_format_, usage_rendertarget);

					if( !m_texture )
					{
						throw std::exception( "render_target::init create_texture(...) failed. ");
					}

					m_surface = m_texture->get_surface(0);

					if (!m_surface)
					{
						throw std::exception( "render_target::init get_surface(...) failed. ");
					}
				}
				else
				{
					m_surface = surface::create_rt(get_device(), m_desc.width_, m_desc.height_,
						m_desc.texture_format_, ms_none, false);

					if(!m_surface)
					{
						throw std::exception( "render_target::ini CreateRenderTarget(...) failed. ");
					}
				}
			}

			if(m_desc.depth_stencil_format_)
			{
				int ds_width;
				int ds_height;

				if (m_desc.use_depth_wh_)
				{
					ds_width = m_desc.depth_width_;
					ds_height = m_desc.depth_height_;
				}
				else
				{
					ds_width = m_desc.width_;
					ds_height = m_desc.height_;
				}

				if (m_desc.create_ds_texture_)
				{
					m_depth_texture = texture::create( get_device(), 
						ds_width, 
						ds_height, 
						1, 
						m_desc.depth_stencil_format_,
						usage_depthstencil);

					if ( !m_depth_texture )
					{
						throw std::exception("render_target::ini CreateTexture(...) failed. ");
					}

					/// get depth texture surface
					m_depth_surface = m_depth_texture->get_surface( 0 );		
					if ( !m_depth_surface )
					{
						throw std::exception( "render_target::ini GetSurfaceLevel(...) failed.");
					}
				}
				else
					throw std::exception("Not implemented!");
				//else
				//{
				//	HRESULT hr = device->CreateDepthStencilSurface( 
				//													ds_width,
				//													ds_height,
				//													D3DFMT_D24S8,
				//													D3DMULTISAMPLE_NONE,
				//													0,
				//													TRUE,
				//													&m_depth_surface,
				//													NULL );

				//	if(FAILED( hr ))
				//	{
				//		throw std::exception( (boost::format("render_target::ini CreateDepthStencilSurface(...) failed. hr=%1%") % DXGetErrorDescription9A(hr)).str().c_str() );
				//	}
				//}
			}
		}


		rt_manager::rt_manager(device& device)
			: device_object(device, true)
		{
			m_cache.reserve(10);
			m_stack.reserve(10);
		}

		rt_manager::~rt_manager()
		{
			for(size_t i = 0; i < m_cache.size(); ++i)
			{
				if (m_cache[i].use_count() > 1)
				{
					//warning("[~rt_manager] some RT are still in use!");
				}
			}

			on_device_lost(); // HACK: to force deleting all device resources
		}


		render_target_ptr rt_manager::get(const rt_desc& d, bool force_create)	
		{
			try
			{
				render_target_ptr rt;

				if( force_create )
				{
					//<any time create./>
					rt = render_target_ptr(new render_target(get_device()));
					rt->m_desc = d;
					rt->reinit();
					rt->lock();
					m_cache.push_back(rt);
				}
				else
				{
					//<create if not exists and not in use./>
					for( render_targets_vector::const_iterator i = m_cache.begin(); i != m_cache.end(); ++i )
					{
						if( (*i)->get_desc() == d
							&& ( !(*i)->is_locked() || (*i).use_count() == 1 ) )
						{
							rt = (*i);
							rt->unlock();
							break;
						}				
					}
					//<if not found in cache - create and push./>
					if( !rt )
					{
						rt = render_target_ptr(new render_target(get_device()));
						rt->m_desc = d;
						rt->reinit();
						m_cache.push_back(rt);
					}		
				}


				return rt;
			}
			catch( std::exception& e )
			{
				std::clog << e.what() << std::endl;
				return render_target_ptr();
			}
		}

		void rt_manager::set( const render_target_ptr& rt, bool& set_color, bool& set_depth )
		{
			if( set_color && rt->get_color_surface() )
			{
				if( !get_device().set_render_target(0, rt->get_color_surface()) )
				{
					throw std::exception( "rt_manager::set: SetRenderTarget: failed. "  );
				}

				set_color = false;
			}

			if( set_depth && rt->get_depth_surface() )
			{
				if( !get_device().set_depth_surface(rt->get_depth_surface()) )
				{
					throw std::exception( "rt_manager::set: SetDepthStencilSurface: failed. " );
				}

				set_depth = false;
			}
		}

		void rt_manager::push(render_target_ptr rt)
		{
			m_stack.push_back(rt);

			bool set_color = true;
			bool set_depth = true;
			set( rt, set_color, set_depth );
		}

		void rt_manager::pop()
		{
			const render_target_ptr& curent = m_stack.back();
			bool set_color = curent->get_color_surface();
			bool set_depth = curent->get_depth_surface();
			m_stack.pop_back();	

			render_targets_vector::const_reverse_iterator ri = m_stack.rbegin(), rend = m_stack.rend();
			for( ; ri != rend && (set_color || set_depth); ++ri )
			{
				set( *ri, set_color, set_depth );
			}	
		}


		void rt_manager::on_device_lost()
		{
			m_stack.clear();

			std::for_each(m_cache.begin(), m_cache.end(), 
				boost::bind(&render_target::on_device_lost, _1));
		}


		void rt_manager::on_device_reset()
		{
			surface_ptr depth_surface = get_device().get_depth_surface();
			assert(depth_surface);

			surface_ptr color_surface = get_device().get_render_target(0);
			assert(color_surface);

			render_target_ptr rt(new render_target(get_device(), color_surface, depth_surface));

			m_stack.push_back(rt);

			try
			{
				std::for_each(m_cache.begin(), m_cache.end(), 
					boost::bind(&render_target::on_device_reset, _1));
			}
			catch( const std::exception& e )
			{
				std::cerr << e.what() << std::endl;
				throw;
			}
		}

		void rt_manager::get_mem_usage( std::vector<std::string>& _list ) const
		{
			//render_targets_vector::const_iterator i =  m_cache.begin(), end = m_cache.end();
			//for( ; i != end; ++i )
			//{
			//	_list.push_back((*i)->get_mem_usage());
			//}
		}

		int rt_manager::get_mem_usage( ) const
		{
			int result(0);
			//render_targets_vector::const_iterator i =  m_cache.begin(), end = m_cache.end();
			//for( ; i != end; ++i )
			//{
			//	result += (*i)->get_mem_usage();
			//}

			return result;
		}
	}
}
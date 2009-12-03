#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <rgde/render/device_object.h>


namespace rgde
{
	namespace render
	{
		struct rt_desc
		{
			// screen_divisor : 0 - fixed size
			//                  1 - full size
			//					From here crop the scene texture so width and height are evenly divisible by 8.	
			//					2 - 1/2 size
			//					4 - 1/4 size
			//					8 - 1/8 size
			int screen_divisor_;

			/// width of the render target surface / render target texture
			int width_;					

			/// height of the render target surface / render target texture
			int height_;				

			/// format of the render target surface / render target texture
			resource::format texture_format_;		

			/// format of the depth stencil surface / depth stencil texture
			resource::format depth_stencil_format_;	
			
			size_t size_shift_;
			bool force_crop_;

			///  use depth_width_ and depth_height_ or width_ and height_ to create depth texture/surface
			bool use_depth_wh_;			
			
			///  width of the depth stencil surface / depth stencil texture
			int depth_width_;			

			///  height of the depth stencil surface / depth stencil texture
			int depth_height_;			

			///  false - create render target surface / true - create render target texture
			bool create_rt_texture_;	

			///  false - create depth stencil surface / true - create depth stencil texture
			bool create_ds_texture_;	

			rt_desc( int width = 0
				, int height = 0
				, resource::format texture_format = resource::a8r8g8b8
				, resource::format depth_stencil_format = resource::d24s8
				, size_t screen_divisor = 1
				, size_t size_shift = 0
				, bool force_crop = false
				, bool use_depth_wh = false
				, int depth_width_ = 0
				, int depth_height_ = 0
				, bool create_rt_texture_ = true
				, bool create_ds_texture_ = false
				);

			bool operator == (const rt_desc& rhs) const;
		};

		struct rt_desc;

		class render_target : public device_object
		{
			friend class rt_manager;
		public:
			~render_target();

			const rt_desc& get_desc() const;

			texture_ptr get_color_texture() const	{ return m_texture;	}
			surface_ptr get_color_surface() const	{ return m_surface; }

			surface_ptr get_depth_surface() const	{ return m_depth_surface; }
			texture_ptr get_depth_texture() const	{ return m_depth_texture; }

			void set_current();//<throw std::exception./>
			
			bool is_locked() const { return m_locked; }
			void lock() { m_locked = true;}
			void unlock() { m_locked = false;}

			bool operator == (const render_target& rhs) const;

			void clear_texture( unsigned long argb_color );

			//std::string get_mem_usage_info() const {}
			//int get_mem_usage() const;

		private:
			render_target(device& device);
			render_target(device& device, surface_ptr tr, surface_ptr depth);

			void reinit();				//<throw std::exception./> <init with current descriptor./>
			void release();

			void on_device_lost();
			void on_device_reset();

		protected:
			rt_desc		m_desc;
			bool		m_locked;

			texture_ptr	m_texture;
			surface_ptr	m_surface;
			texture_ptr	m_depth_texture;
			surface_ptr	m_depth_surface;	
		};
		
		typedef boost::shared_ptr<class render_target> render_target_ptr;

		class rt_manager : public device_object
		{	
		public :
			rt_manager(device& device);
			~rt_manager();

			render_target_ptr get(const rt_desc& d, bool force_create = false );

			void push(render_target_ptr rt);//<throw std::exception./>
			void pop();//<throw std::exception./>

			void get_mem_usage( std::vector<std::string>& list ) const;
			int get_mem_usage() const;

		private:
			void on_device_lost();
			void on_device_reset();

			void set( const render_target_ptr& rt, bool& set_color, bool& set_depth );//<throw std::exception./>

			typedef std::vector<render_target_ptr> render_targets_vector;	
			render_targets_vector m_cache;
			render_targets_vector m_stack;
		};
	}
}
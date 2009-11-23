#pragma once

#include <rgde/math/camera.h>
#include <rgde/render/font.h>

#include <rgde/base/singelton.h>

namespace render
{
	class device_object;
	class lines3d;
	class lines2d;

	typedef boost::shared_ptr<lines2d> lines2d_ptr;
	typedef boost::shared_ptr<lines3d> lines3d_ptr;

	class render_device
	{
	protected:
		render_device();
		~render_device();

	public:
		void					on_lost();
		void					on_reset();

		void					add_object(device_object* obj);
		void					remove_object(device_object* obj);

		void					set_camera(const math::camera_ptr& cam){m_cam = cam; }
		const math::camera_ptr&	get_camera() const {return m_cam;}

		void					set_shader_flags(unsigned long flag)	{m_shaderFlags = flag;}
		unsigned long			get_shader_flags() const				{return m_shaderFlags;}

		void					add_statistics( unsigned verts, unsigned tris);
		void					reset_statistics();

		inline unsigned			get_tris()	const {return m_triangles;}
		inline unsigned			get_verts()	const {return m_verts;}

		math::vec2f				getBackBufferSize();		

		void					draw_wired_floor(float size, unsigned num = 20, const math::Color& color = math::Green);

		float                   get_fps(float abs_time) const;
		void                    draw_fps(float abs_time, const font_ptr& font);
		void                    showStatistics(const font_ptr& font);

        //->
        void                    set_clear_color (math::Color color) {m_clear_color = color;}
        const math::Color&      get_clear_color () const {return m_clear_color;}
        //-<

		lines3d&				get_lines3d();
		lines2d&				get_lines2d();

		static render_device&	get();
		static bool				is_created();

	protected:
		void init();

	protected:
        //->
        math::Color             m_clear_color;
        //-<

		unsigned long			m_shaderFlags;
		math::camera_ptr		m_cam;

		unsigned				m_verts;
		unsigned				m_triangles;

		typedef std::list<device_object* > device_objects;
		device_objects			m_objects;

		lines2d_ptr				m_lines2d;
		lines3d_ptr				m_lines3d;

		static render_device*	ms_instance;
		static bool				ms_is_created;
	};

	class device_object
	{
	public:
								device_object();
		virtual					~device_object();

		virtual void			onLostDevice() = 0;
		virtual void			onResetDevice() = 0;
	};
}
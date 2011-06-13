#pragma once

#include <rgde/math/camera.h>


namespace render
{
	typedef boost::shared_ptr<class render_texture> render_texture_ptr;

	struct viewport : math::Rect
 	{
		viewport(int x, int y, int width, int height)
			: Rect((float)x, (float)y, (float)width, (float)height)
		{}
    };

	class render_camera;
	typedef boost::intrusive_ptr<class render_camera> camera_ptr;

	class render_camera : public math::base_camera
	{
	public:		
		~render_camera();

		void activate();

		void setDepthStencilTarget(const render_texture_ptr& target, float clearDepth);
		void setColorTarget(const render_texture_ptr& target, const math::Color& clearColor);
		
		static camera_ptr create();
		static camera_ptr create(uint priority, const viewport& viewport);

		void priority(uint priority);
		uint priority() const;

	private:
		render_camera(uint priority, const viewport& viewport);

	private:		
		viewport		m_viewport;
		math::Color		m_clear_color;
		float			m_clear_depth;
		render_texture_ptr	m_color_target;
		render_texture_ptr	m_pDepthStencilTarget;
		unsigned		m_priority;
	};	
}
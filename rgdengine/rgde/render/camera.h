#pragma once

#include <rgde/math/camera.h>


namespace render
{
	typedef boost::shared_ptr<class render_texture> PRenderTexture;

	struct viewport : math::Rect
 	{
		viewport(int x, int y, int width, int height)
			: Rect((float)x, (float)y, (float)width, (float)height)
		{}
    };

	class render_camera;
	typedef boost::intrusive_ptr<class render_camera> camera_ptr;

	class render_camera : public math::BaseCamera
	{
	public:		
		~render_camera();

		void activate();

		void setDepthStencilTarget(const PRenderTexture& pTarget, float clearDepth);
		void setColorTarget(const PRenderTexture& pTarget, const math::Color& clearColor);
		
		static camera_ptr render_camera::create();
		static camera_ptr render_camera::create(uint priority, const viewport& viewport);

		void	 setPriority(uint priority);
		uint getPriority() const;

	private:
		render_camera(uint priority, const viewport& viewport);

	private:		
		viewport		m_viewport;
		math::Color		m_clear_color;
		float			m_clear_depth;
		PRenderTexture	m_pColorTarget;
		PRenderTexture	m_pDepthStencilTarget;
		unsigned		m_priority;
	};	
}
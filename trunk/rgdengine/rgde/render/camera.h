#pragma once

#include <rgde/math/camera.h>


namespace render
{
	typedef boost::shared_ptr<class IRenderTexture> PRenderTexture;

	struct ViewPort : math::Rect
 	{
		ViewPort(int x, int y, int width, int height)
			: Rect((float)x, (float)y, (float)width, (float)height)
		{}
    };

	class RenderCamera;
	typedef boost::intrusive_ptr<class RenderCamera> PCamera;

	class RenderCamera : public math::BaseCamera
	{
	public:		
		~RenderCamera();

		void activate();

		void setDepthStencilTarget(const PRenderTexture& pTarget, float clearDepth);
		void setColorTarget(const PRenderTexture& pTarget, const math::Color& clearColor);
		
		static PCamera RenderCamera::Create();
		static PCamera RenderCamera::Create(uint priority, const ViewPort& viewport);

		void	 setPriority(uint priority);
		uint getPriority() const;

	private:
		RenderCamera(uint priority, const ViewPort& viewport);

	private:		
		ViewPort		m_Viewport;
		math::Color		m_ClearColor;
		float			m_ClearDepth;
		PRenderTexture	m_pColorTarget;
		PRenderTexture	m_pDepthStencilTarget;
		unsigned		m_Priority;
	};	
}
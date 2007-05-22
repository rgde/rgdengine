#pragma once

#include <rgde/math/mathCamera.h>


namespace render
{
	typedef boost::shared_ptr<class IRenderTexture> PRenderTexture;

	struct SViewPort : math::Rect
 	{
		SViewPort(int x, int y, int width, int height)
			: Rect((float)x, (float)y, (float)width, (float)height)
		{}
    };

	class CRenderCamera;
	typedef boost::intrusive_ptr<class CRenderCamera> PCamera;

	class CRenderCamera : public math::CBaseCamera
	{
	public:		
		~CRenderCamera();

		void activate();

		void setDepthStencilTarget(const PRenderTexture& pTarget, float clearDepth);
		void setColorTarget(const PRenderTexture& pTarget, const math::Color& clearColor);
		
		static PCamera CRenderCamera::Create();
		static PCamera CRenderCamera::Create(uint priority, const SViewPort& viewport);

		void	 setPriority(uint priority);
		uint getPriority() const;

	private:
		CRenderCamera(uint priority, const SViewPort& viewport);

	private:		
		SViewPort		m_Viewport;
		math::Color		m_ClearColor;
		float			m_ClearDepth;
		PRenderTexture	m_pColorTarget;
		PRenderTexture	m_pDepthStencilTarget;
		unsigned		m_Priority;
	};	
}
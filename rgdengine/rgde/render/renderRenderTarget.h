#pragma once

#include <rgde/render/renderTexture.h>

namespace render
{
	struct RenderTargetInfo
	{
		math::Vec2i		size;
		TextureFormat	format;
	};

	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget(){}
		virtual void activate() = 0;
	};

	class IRenderTexture;
	typedef boost::shared_ptr<IRenderTexture> PRenderTexture;


	class IRenderTexture : public IRenderTarget, public ITexture
	{
	public:
		static PRenderTexture Create(const RenderTargetInfo& params);
	};
}
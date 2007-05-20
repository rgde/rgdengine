#pragma once

#include "render/renderTexture.h"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

namespace render
{
	struct SRenderTargetInfo
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
		static PRenderTexture Create(const SRenderTargetInfo& params);
	};
}
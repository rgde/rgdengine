#pragma once

#include <rgde/render/texture.h>

namespace render
{
	struct RenderTargetInfo
	{
		math::Vec2i		size;
		TextureFormat	format;
	};

	class render_target
	{
	public:
		virtual ~render_target(){}
		virtual void activate() = 0;
	};

	class render_texture;
	typedef boost::shared_ptr<render_texture> PRenderTexture;


	class render_texture : public render_target, public texture
	{
	public:
		static PRenderTexture create(const RenderTargetInfo& params);
	};
}
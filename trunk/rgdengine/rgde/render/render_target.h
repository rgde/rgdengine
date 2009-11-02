#pragma once

#include <rgde/render/texture.h>

namespace render
{
	struct render_target_info
	{
		math::Vec2i		size;
		texture_format	format;
	};

	class render_target
	{
	public:
		virtual ~render_target(){}
		virtual void activate() = 0;
	};

	class render_texture;
	typedef boost::shared_ptr<render_texture> render_texture_ptr;


	class render_texture : public render_target, public texture
	{
	public:
		static render_texture_ptr create(const render_target_info& params);
	};
}
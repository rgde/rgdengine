#pragma once

#include "rect.h"


namespace gui
{
	class Imageset;
	class Texture;

	class  Image
	{
	public:
		enum BLEND_OPS
		{
			BLEND_ADD,
			BLEND_MODULATE
		};
		enum ImageOps
		{
			Stretch,
			Tile
		};

		Image(Texture& tex, const std::string& name, const Rect& pixelArea, 
			BLEND_OPS blend = BLEND_MODULATE, const Imageset* owner = NULL);

		Image(const Image& img);
		~Image();

		const std::string&	getName() const {return m_name;}
		const Imageset*	getImageset() const {return m_owner;}

		Texture&	texture;

		Rect		pixel_rect;
		Rect		uv_rect;
		BLEND_OPS	blend_mode;

	private:
		Image& operator=(const Image&) { return *this; }
		
		const Imageset*	m_owner;
		std::string	m_name;
	};

Image::ImageOps StringToImageOps(const std::string& str);
std::string ImageOpsToString(Image::ImageOps op);
}

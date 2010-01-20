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

		const Rect& getPixelRect() const {return m_pixelArea;}
		const Rect& getUVRect() const {return m_UVArea;}

		Texture& getTexture() const {return m_texture;}

		BLEND_OPS getBlend() const { return m_blend; }

	private:
		Image& operator=(const Image&) { return *this; }
		Texture& m_texture;
		const Imageset*	m_owner;

		BLEND_OPS m_blend;

		Rect			m_pixelArea;
		Rect			m_UVArea;

		std::string	m_name;
	};

Image::ImageOps StringToImageOps(const std::string& str);
std::string ImageOpsToString(Image::ImageOps op);
}

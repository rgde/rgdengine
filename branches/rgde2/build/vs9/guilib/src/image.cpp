#include "stdafx.h"

#include "image.h"
#include "texture.h"

namespace gui
{
Image::Image(Texture& tex, const std::string& name, const Rect& pixelArea, 
	BLEND_OPS blend, const Imageset* owner) :
	m_owner(owner),	
	texture(tex),	
	uv_rect(pixelArea),
	pixel_rect(pixelArea),
	blend_mode(blend_mode),	
	m_name(name)
{
	uv_rect *= texture.getSize();
}

Image::Image(const Image& img) :
	texture(img.texture),
	m_owner(img.m_owner),
	pixel_rect(img.pixel_rect),
	uv_rect(img.uv_rect),
	blend_mode(img.blend_mode),
	m_name(img.m_name)
{
}

Image::~Image()
{
}

Image::ImageOps StringToImageOps(const std::string& str)
{
	if(str == "Tile" || str == "tile")
		return Image::Tile;
	if(str == "Stretch" || str == "stretch")
		return Image::Stretch;
	
	return Image::Stretch;
}

std::string ImageOpsToString(Image::ImageOps op)
{
	switch(op)
	{
	case Image::Tile:
		return "Tile";
		break;
	case Image::Stretch:
	default:
		return "Stretch";
		break;
	}
	return "Stretch";
}
}
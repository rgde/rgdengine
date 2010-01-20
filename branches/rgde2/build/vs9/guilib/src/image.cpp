#include "stdafx.h"

#include "image.h"
#include "texture.h"

namespace gui
{
Image::Image(Texture& tex, const std::string& name, const Rect& pixelArea, 
	BLEND_OPS blend, const Imageset* owner) :
	m_owner(owner),	
	m_texture(tex),	
	m_pixelArea(pixelArea),
	m_UVArea(pixelArea),
	m_blend(blend),	
	m_name(name)
{
	m_UVArea *= m_texture.getSize();
}

Image::Image(const Image& img) :
	m_texture(img.m_texture),
	m_owner(img.m_owner),
	m_pixelArea(img.m_pixelArea),
	m_UVArea(img.m_UVArea),
	m_blend(img.m_blend),
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
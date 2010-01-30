
#pragma once

#include "font.h"

namespace gui
{

class Renderer;
class script_object;

class RenderHelper
{
public:
	RenderHelper(Renderer& r) : m_render(r) {}
	~RenderHelper() {}

	void drawImage(const Image* img, Rect& dest, Rect& clip, Color col, Image::ImageOps h, Image::ImageOps v)
	{
		assert(img);
		m_render.draw(*img, dest, 0.f, clip, ColorRect(col), h, v);
	}
	void drawText(Font* font, std::string text, Rect& area, Rect& clip, Font::TextFormatting fmt, Color col)	
	{
		if(font && !text.empty())
			font->drawText(text, area, 0.f, clip, fmt, ColorRect(col));
	}

protected:
	Renderer& m_render;
};


}
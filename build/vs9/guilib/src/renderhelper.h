
#pragma once

#include "font.h"

namespace gui
{

class Renderer;
class script_object;

class RenderHelper
{
public:
	explicit RenderHelper(Renderer& r) : m_render(r) {}
	~RenderHelper() {}

	void draw(const Image* img, Rect& dest, Rect& clip, Color col, Image::ImageOps h, Image::ImageOps v)
	{
		assert(img);
		m_render.draw(*img, dest, 0.f, clip, ColorRect(col), h, v);
	}
	void draw(Font* font, std::string text, Rect& area, Rect& clip, Font::TextFormatting fmt, Color col)	
	{
		if(font && !text.empty())
			font->draw(text, area, 0.f, clip, fmt, ColorRect(col));
	}

	// for Script binding
	void draw_image(const Image* img, Rect& dest, Rect& clip, Color col, Image::ImageOps h, Image::ImageOps v) {
		draw(img, dest, clip, col, h, v);
	}
	void draw_text(Font* font, std::string text, Rect& area, Rect& clip, Font::TextFormatting fmt, Color col) {
		draw(font, text, area, clip, fmt, col);
	}

protected:
	Renderer& m_render;
};


}
#pragma once

#include <engine/core/math/forward.h>

#include <engine/render/device_object.h>

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace rgde
{
	namespace render
	{
		class device;

		class font : public device_object
		{
			class font_impl;
			typedef boost::scoped_ptr<font_impl> pimpl;
		public:
			enum weight
			{
				thin,
				extra_light,
				light,
				normal,
				medium,
				semi_bold,
				bold,
				extra_bold,
				heavy
			};

			enum format
			{				
				top = 0,			// Top-justifies text.
				left = 0,			// Aligns text to the left. 
				center,			// Centers text horizontally in the rectangle.
				right,			// Aligns text to the right. 
				vcenter,		// Centers text vertically (single line only). 
				bottom,			// Justifies the text to the bottom of the rectangle. This value must be 
								// combined with SingleLine.

				word_break = 0,		// Breaks words. Lines are automatically broken between words if a word would 
								// extend past the edge of the rectangle specified by the pRect parameter. A 
								// carriage return/line feed sequence also breaks the line.

				single_line,	// Displays text on a single line only. Carriage returns and line feeds do not 
								// break the line. 

				ExpandTabs,		// Expands tab characters. The default number of characters per tab is eight

				NoClip,			// Draws without clipping. IFont::renderText is somewhat faster when NoClip is 
			};

			static font_ptr create(device& dev, size_t height, const std::wstring& name, weight w = normal);

			void render(const std::wstring& text, const math::rect& rc, unsigned int color = 0xFFFFFFFF);
			void render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow = true);
			void render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow, int flags);

			math::rect measure_test( const std::wstring& text, int flags = 0);

			~font();

		private:
			font(device& dev, size_t height, const std::wstring& name, weight w = normal);			

		private:
			pimpl m_pimpl;
		};
	}
}
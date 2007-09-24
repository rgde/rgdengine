#pragma once

#include <engine/core/math/forward.h>

#include <string>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace rgde
{
	namespace render
	{
		class device;

		class font
		{
			class font_impl;
			typedef boost::scoped_ptr<font_impl> pimpl;
		public:
			enum Weight
			{
				DontCare    =   0,
				Thin        =   100,
				ExtraLight  =   200,
				Light       =   300,
				Normal      =   400,
				Medium      =   500,
				SemiBold    =   600,
				Bold        =   700,
				ExtraBold   =   800,
				Heavy       =   900
			};

			enum Format
			{			
				Top        =   0x00000000, // Top-justifies text. 			
				Left       =   0x00000000, // Aligns text to the left. 
				Center     =   0x00000001, // Centers text horizontally in the rectangle.
				Right      =   0x00000002, // Aligns text to the right. 
				VCenter    =   0x00000004, // Centers text vertically (single line only). 
				Bottom     =   0x00000008, // Justifies the text to the bottom of the rectangle. This value must be 
				// combined with SingleLine.

				WordBreak  =   0x00000010, // Breaks words. Lines are automatically broken between words if a word would 
				// extend past the edge of the rectangle specified by the pRect parameter. A 
				// carriage return/line feed sequence also breaks the line.

				SingleLine =   0x00000020, // Displays text on a single line only. Carriage returns and line feeds do not 
				// break the line. 

				ExpandTabs =   0x00000040, // Expands tab characters. The default number of characters per tab is eight

				NoClip     =   0x00000100, // Draws without clipping. IFont::renderText is somewhat faster when NoClip is 
				// used.
			};

			font(device& dev, size_t height, const std::wstring& name, Weight weigh = Normal);
			~font();

			void render(const std::wstring& text, const math::rect& rc, unsigned int color = 0xFFFFFFFF);
			void render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow = true);
			void render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow, int flags);

			math::rect measure_test( const std::wstring& text, int flags = 0);

			const device& get_device() const;
			device& get_device();

			font_impl& get_impl();
			const font_impl& get_impl() const;

		private:
			pimpl m_pimpl;
		};
	}
}
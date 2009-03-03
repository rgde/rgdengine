#pragma once

#include <rgde/math/types3d.h>
#include <rgde/base/smart_ptr_helpers.h>

namespace render
{
	typedef boost::shared_ptr<class IFont> font_ptr;

	class IFont
	{
	public:
		enum FontWeight
		{
			DontCare     =   0,
			Thin         =   100,
			ExtraLight   =   200,
			Light        =   300,
			Normal       =   400,
			Medium       =   500,
			SemiBold     =   600,
			Bold         =   700,
			ExtraBold    =   800,
			Heavy        =   900
		};

		enum FontFormat
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
	public:
		static font_ptr create(int height, const std::wstring& name, FontWeight font_weigh = Normal);

		virtual void disableDelayedRender(bool b) = 0;
		void render(const std::wstring& text, const math::Rect& rect, unsigned int color = 0xFFFFFFFF);
		void render(const std::wstring& text, const math::Rect& rect, unsigned int color, bool isDrawShadow);
		virtual void render(const std::wstring& text, const math::Rect& rect, unsigned int color, bool isDrawShadow, int flags) = 0;

		virtual math::Rect getRect( const std::wstring& text, int flags = 0) = 0;

		virtual ~IFont(){}
	};
}
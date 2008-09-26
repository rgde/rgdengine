//======== Copyright © 2004 - 2005, viv software, All rights reserved =========
//
// File: drawing.h
// Author: Vashchaev 'viv' Ivan
// Purpose: $$$
//
//=============================================================================

#ifndef FORMS_DRAWING_H
#define FORMS_DRAWING_H

namespace forms
{
	namespace Drawing
	{
		// Point
		class Point : public POINT
		{
		public:
			Point()
			{
				this->x = 0;
				this->y = 0;
			}

			Point(int x, int y)
			{
				this->x = x;
				this->y = y;
			}

			Point(const Point &p)
			{
				this->x = p.x;
				this->y = p.y;
			}
		};

		// Size
		class Size : public SIZE
		{
		public:
			Size()
			{
				this->cx = 0;
				this->cy = 0;
			}

			Size(int cx, int cy)
			{
				this->cx = cx;
				this->cy = cy;
			}

			Size(const Size &p)
			{
				this->cx = p.cx;
				this->cy = p.cy;
			}
		};

		// Rectangle
		class Rectangle : public RECT
		{
		public:
			Rectangle()
			{
				this->top = 0;
				this->left = 0;
				this->right = 0;
				this->bottom = 0;
			}

			Rectangle(int top, int left, int right, int bottom)
			{
				this->top = top;
				this->left = left;
				this->right = right;
				this->bottom = bottom;
			}

			Rectangle(const Point &tl, const Point &br)
			{
				this->top = tl.y;
				this->left = tl.x;
				this->right = br.x;
				this->bottom = br.y;
			}

			Rectangle(const Point &p, const Size &s)
			{
				this->top = p.y;
				this->left = p.x;
				this->right = p.x + s.cx;
				this->bottom = p.y + s.cy;
			}

			Rectangle(const Rectangle &r)
			{
				this->top = r.top;
				this->left = r.left;
				this->right = r.right;
				this->bottom = r.bottom;
			}

			int GetWidth() const
			{
				return right - left;
			}

			int GetHeight() const
			{
				return bottom - top;
			}

			Point GetTopLeft() const
			{
				return Point(left, top);
			}

			Point GetBottomRight() const
			{
				return Point(right, bottom);
			}

			Size GetSize() const
			{
				return Size(GetWidth(), GetHeight());
			}
		};

		const Rectangle DefaultRect	= Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, 0, 0);

		// Base class for GDI object wrappers
		template <typename HandleT, bool Managed>
		class GDI
		{
		public:
			HandleT	m_Handle;

			GDI()
				: m_Handle(NULL)
			{
			}

			~GDI()
			{
				if (Managed && m_Handle)
				{
					::DeleteObject(m_Handle);
				}
			}

			GDI<HandleT, Managed> & operator=(HandleT Handle)
			{
				Attach(Handle);
				return *this;
			}

			void Attach(HandleT Handle)
			{
				if (Managed && m_Handle && m_Handle != Handle)
				{
					::DeleteObject(m_Handle);
				}
				m_Handle = Handle;
			}

			HandleT Detach()
			{
				HandleT	Temp			= m_Handle;
				m_Handle = NULL;
				return Temp;
			}

			operator HandleT() const
			{
				return m_Handle;
			}
		};

		// Font
		template <bool Managed>
		class FontT : public GDI<HFONT, Managed>
		{
		public:
		};

		typedef FontT<true> Font;
		typedef FontT<false> FontHandle;

		// Pen
		template <bool Managed>
		class PenT : public GDI<HPEN, Managed>
		{
		public:
		};

		typedef PenT<true> Pen;
		typedef PenT<false> PenHandle;

		// Brush
		template <bool Managed>
		class BrushT : public GDI<HBRUSH, Managed>
		{
		public:
		};

		typedef BrushT<true> Brush;
		typedef BrushT<false> BrushHandle;

		// Bitmap
		template <bool Managed>
		class BitmapT : public GDI<HBITMAP, Managed>
		{
		public:
		};

		typedef BitmapT<true> Bitmap;
		typedef BitmapT<false> BitmapHandle;

		// Icon
		template <bool Managed>
		class IconT : public GDI<HICON, Managed>
		{
		public:
		};

		typedef IconT<true> Icon;
		typedef IconT<false> IconHandle;

		// Canvas
		template <bool Managed>
		class CanvasT : public GDI<HDC, Managed>
		{
		public:
		};

		typedef CanvasT<true> Canvas;
		typedef CanvasT<false> CanvasHandle;
	}
}

#endif
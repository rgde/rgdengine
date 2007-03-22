//font.h
#pragma once
#include "types.h"
#include <boost/shared_ptr.hpp>

namespace ui
{
    //интрефейс шрифта
    struct IFont
    {
        virtual ~IFont(){}

        enum Aligment //выравнивание выводимой строки внутри прямоугольника вывода
        {
            Top     = 0x00000000,
            Left    = 0x00000001,
            Center  = 0x00000002,
            Right   = 0x00000004,
            VCenter = 0x00000008,
            Bottom  = 0x0000000F
        };

        virtual void draw        (const std::wstring &text, const Rect &r, const Color &color, int flags = 0) = 0;
        virtual Rect measureText (const std::wstring &text) = 0;

        virtual const std::wstring& getFontFamily () const = 0;
        virtual int                 getHeight     () const = 0;
        virtual bool                isItalic      () const = 0;
        virtual bool                isBold        () const = 0;
        virtual bool                isFixed       () const = 0;
    };

    //смартпоинтер
    typedef boost::shared_ptr<IFont> PFont;

} //namespace ui
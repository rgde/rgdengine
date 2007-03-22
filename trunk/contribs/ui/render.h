//render.h
#pragma once
#include "types.h"
#include <boost/shared_ptr.hpp>

namespace ui
{
    //интрефейс рендера
    struct IRender
    {
        virtual ~IRender () {}
        virtual void drawBitmap (const PBitmap &bitmap, const Rect &r, const Color &color) = 0;
        virtual void drawText   (const PFont &font, const std::wstring &text, const Rect &r, const Color &color, int flags = 0) = 0;
    };

    //смартпоинтер
    typedef boost::shared_ptr<IRender> PRender;

} //namespace ui
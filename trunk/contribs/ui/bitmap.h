//bitmap.h
#pragma once
#include "types.h"
#include <boost/shared_ptr.hpp>

namespace ui
{
    //интрефейс картинки
    struct IBitmap
    {
        virtual ~IBitmap (){}
        virtual Size getSize   () const = 0;
        virtual int  getHeight () const = 0;
        virtual int  getWidth  () const = 0;
        virtual void draw      (const Rect& r, const Rect& uv = Rect(), const Color& color = 0xFFFFFFFF) = 0;
    };

    //смартпоинтер
    typedef boost::shared_ptr<IBitmap> PBitmap;

} //namespace ui
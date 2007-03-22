//types.h
#pragma once
#include <boost/shared_ptr.hpp>

namespace ui
{
    //предопределения
    class  CSystem;
    class  CCursor;
    struct IRender;
    struct IBitmap;
    struct IFont;

    //смартпоинтеры
    typedef boost::shared_ptr<CSystem> PSystem;
    typedef boost::shared_ptr<CCursor> PCursor;
    typedef boost::shared_ptr<IRender> PRender;
    typedef boost::shared_ptr<IBitmap> PBitmap;
    typedef boost::shared_ptr<IFont  > PFont;

    //вспомогательные типы
    struct Color;
    struct Rect;
    struct Size;
    struct Point;
    //...

} //namespace ui
//types.h
#pragma once

namespace ui
{

    class Color;
    class Size;
    class Point;
    class Rect;

    //цвет
    class Color
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    //рамер
    class Size
    {
        float width;
        float height;
    };

    //точка
    class Point
    {
        float x;
        float y;
    };

    //прямоугольник
    class Rect
    {
        float x;
        float y;
        float width;
        float height;
    };

} //namespace ui
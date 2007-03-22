//factory.h
#pragma once
#include "types.h"

namespace ui
{
    //интрефейс фабрики для рендера
    struct IFactory
    {
        virtual ~IFactory (){}
        virtual PBitmap createBitmap (const std::wstring& sFilePath) = 0;
        virtual PFont   createFont   (const std::wstring& sFontFamily, int height, bool italic = false, bool bold = false, bool fixed = false);
    };

} //namespace ui
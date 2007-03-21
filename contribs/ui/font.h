//font.h
#pragma once
#include "render.h"
#include "types.h"

namespace ui
{

    //бфзовый класс для шрифтов
    class CFont
    {
    public:
        virtual ~CFont() {}
        const std::wstring& getName() const {return m_sName;}
        CRender&            getRender() {return m_rRender;}
        virtual void        render(const Point&, const std::wstring&) = 0;
        //рендер:
        //* в прямоугольник, с выравниванием (3 позиции) по высоте и ширине
        //* базовая линия
        //* получить охватывающий прямоугольник
        //...

    private:
        friend CFont* CRender::createFont(const std::wstring&);
        CFont(const std::wstring &sName, CRender &rRender): m_sName(sName), m_rRender(rRender) {}

        std::wstring  m_sName;
        CRender      &m_rRender;
        //...

        CFont(const CFont&);
        CFont& operator=(const CFont&);
    };

} //namespace ui
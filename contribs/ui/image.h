//image.h
#pragma once
#include "render.h"
#include "types.h"

namespace ui
{

    //базовый класс изображения
    class CImage
    {
    public:
        virtual ~CImage() {}
        const std::wstring& getName() const {return m_sName;}
        CRender&            getRender() {return m_rRender;}
        const Size&         getSize() const {return m_size;}
        virtual void        render(const Rect &dst, const Rect &src) = 0;

    private:
        friend CImage* CRender::createImage(const std::wstring&);
        CImage(const std::wstring &sName, CRender &rRender): m_sName(sName), m_rRender(rRender) {}

        std::wstring  m_sName;
        CRender      &m_rRender;
        Size          m_size;

        CImage(const CImage&);
        CImage& operator=(const CImage&);
    };

} //namespace ui
//render.cpp
#include <string>
#include "render.h"
#include "image.h"
#include "font.h"

namespace ui
{
    CRender::~CRender()
    {
        //удалим изображения
        Images::iterator i = m_images.begin();
        while (i != m_images.end())
        {
            delete (*i);
            i = m_images.erase(i);
        }

        //удалим шрифты
        Fonts::iterator j = m_fonts.begin();
        while (j != m_fonts.end())
        {
            delete (*j);
            j = m_fonts.erase(j);
        }
    }

    CImage* CRender::addImage (const std::wstring &sName)
    {
        CImage *img = getImage(sName);
        if (img)
            return img;

        img = createImage(sName);
        m_images.push_back(img);
        return img;
    }

    CFont* CRender::addFont (const std::wstring &sName)
    {
        CFont *fnt = getFont(sName);
        if (fnt)
            return fnt;

        fnt = createFont(sName);
        m_fonts.push_back(fnt);
        return fnt;
    }

    CImage* CRender::getImage (const std::wstring &sName)
    {
        Images::iterator i = m_images.begin();
        while (i != m_images.end())
        {
            if ((*i)->getName().compare(sName))
                return (*i);
            ++i;
        }
        return 0;
    }

    CFont* CRender::getFont (const std::wstring &sName)
    {
        Fonts::iterator i = m_fonts.begin();
        while (i != m_fonts.end())
        {
            if ((*i)->getName() == sName)
                return (*i);
            ++i;
        }
        return 0;
    }

    void CRender::delImage (const std::wstring &sName)
    {
        Images::iterator i = m_images.begin();
        while (i != m_images.end())
        {
            if ((*i)->getName() == sName)
            {
                delete (*i);
                return;
            }
            ++i;
        }
    }

    void CRender::delFont (const std::wstring &sName)
    {
        Fonts::iterator i = m_fonts.begin();
        while (i != m_fonts.end())
        {
            if ((*i)->getName() == sName)
            {
                delete (*i);
                return;
            }
            ++i;
        }
    }

} //namespace ui
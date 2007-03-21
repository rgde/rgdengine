//render.h
#pragma once
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace ui
{

    class CImage;
    class CFont;

    //рендер для ui
    class CRender: public boost::noncopyable
    {
    public:
        virtual ~CRender();

        CImage* addImage (const std::wstring &sName);
        CFont*  addFont  (const std::wstring &sName);

        CImage* getImage (const std::wstring &sName);
        CFont*  getFont  (const std::wstring &sName);

        void    delImage (const std::wstring &sName);
        void    delFont  (const std::wstring &sName);

    protected:
        friend CImage;
        friend CFont;
        virtual CImage* createImage(const std::wstring &sName) = 0;
        virtual CFont*  createFont (const std::wstring &sName) = 0;

    private:
        typedef std::list<CImage*> Images;
        typedef std::list<CFont*>  Fonts;

        Images m_images;
        Fonts  m_fonts;
    };

    //смартпоинтер
    typedef boost::shared_ptr<CRender> PRender;

} //namespace ui
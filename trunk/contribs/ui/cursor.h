//cursor.h
#pragma once
#include "types.h"
#include <boost/shared_ptr.hpp>

namespace ui
{

    //графический курсор
    class CCursor
    {
    public:
        CCursor (PBitmap &pBitmap, const Point &hotspot = Point(0,0));
        virtual ~CCursor ();
        
        //позиция курсора
        const Point& getPosition () const;
        void         setPosition (const Point &position);

        //видимость
        bool isVisible  () const;
        void setVisible (bool visible);

        //отрисовка
        virtual void render ();

    private:
        bool    m_bVisible; //курсор видим/невидим 
        Point   m_position; //координаты курсора
        Point   m_hotspot;  //координаты "острия" курсора
        PBitmap m_pBitmap;  //рисунок курсора
    };

    //смартпоинтер
    typedef boost::shared_ptr<CCursor> PCursor;

} //namespace ui
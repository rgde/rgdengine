//cursor.h
#include "cursor.h"

namespace ui
{

    CCursor::CCursor (PBitmap &pBitmap, const Point &hotspot):
        m_bVisible (true),
        m_hotspot  (hotspot),
        m_position (Point(0,0)),
        m_pBitmap  (pBitmap)
    {
    }

    ~CCursor::CCursor ()
    {
    }

    const Point& CCursor::getPosition () const
    {
        return m_position;
    }

    void CCursor::setPosition (const Point &position)
    {
        m_position = position;
    }

    bool CCursor::isVisible () const
    {
        return m_bVisible;
    }

    void CCursor::setVisible (bool visible)
    {
        m_bVisible = visible;
    }

    virtual void CCursor::render ()
    {
        //todo: m_pBitmap->render();
        //...
    }

} //namespace ui
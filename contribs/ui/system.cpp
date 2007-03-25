//system.h
#include "system.h"

namespace ui
{

    CSystem::CSystem (PRender pRender): m_pRender(pRender)
    {
    }

    CSystem::~CSystem ()
    {
        Widgets::iterator i = m_widgets.begin();
        while (i != m_widgets.end())
        {
            delete (*i);
            i = m_widgets.erase(i);
        }
    }

    CWidget* CSystem::find (const std::string &sName)
    {
        Widgets::iterator i = m_widgets.begin();
        while (i != m_widgets.end())
        {
            if ((*i)->getName() == sName)
                return (*i);
            ++i;
        }
        return 0;
    }

    CWidget* CSystem::remove (const std::string &sName)
    {
        Widgets::iterator i = m_widgets.begin();
        while (i != m_widgets.end())
        {
            if ((*i)->getName() == sName)
            {
                delete (*i);
                return;
            }
            ++i;
        }
    }

    CWidget* CSystem::remove (const CWidget *pWidget)
    {
        if (!pWidget)
            return;

        Widgets::iterator i = m_widgets.begin();
        while (i != m_widgets.end())
        {
            if ((*i) == pWidget)
            {
                delete (*i);
                return;
            }
            ++i;
        }
    }

    void CSystem::injectKey (keys::Keys key, bool pressed)
    {
        //...
    }

    void CSystem::injectChar (wchar_t ch)
    {
        //...
    }

    void CSystem::injectMouseMove (const Point &pos)
    {
        //...
    }

    void CSystem::injectMouseButton (const Point &pos, keys::MouseButtons button, bool pressed)
    {
        //...
    }

    void CSystem::injectMouseButtonDblClick (const Point &pos, keys::MouseButtons button)
    {
        //...
    }

    PCursor CSystem::getCursor ()
    {
        return m_pCursor;
    }

    void CSystem::setCursor (PCursor &pCursor)
    {
        m_pCursor = pCursor;
    }

    void CSystem::update (float dt)
    {
        //...
    }

    void CSystem::render ()
    {
        //...
    }

    std::string CSystem::generateWidgetName()
    {
        //...
    }

} //namespace ui
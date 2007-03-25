//system.h
#pragma once
#include "types.h"
#include "keys.h"
#include <string>
#include <list>

namespace ui
{

    //система ui
    class CSystem
    {
    public:
        CSystem (PRender pRender);
       ~CSystem ();

        //виджеты
        template <typename Widget>
        Widget*  create (const std::string &sName);
        CWidget* find   (const std::string &sName);
        CWidget* remove (const std::string &sName);
        CWidget* remove (const CWidget *pWidget);

        //события
        void injectKey                 (keys::Keys key, bool pressed);
        void injectChar                (wchar_t ch);
        void injectMouseMove           (const Point &pos);
        void injectMouseButton         (const Point &pos, keys::MouseButtons button, bool pressed);
        void injectMouseButtonDblClick (const Point &pos, keys::MouseButtons button);

        //курсор
        PCursor getCursor ();
        void    setCursor (PCursor &pCursor);
        
        //обновление состояния и рендер
        void update (float dt);
        void render ();

    protected:
        std::string generateWidgetName();

    private:
        typedef std::list<CWidget*> Widgets;

        PRender m_pRender;
        PCursor m_pCursor;
        Widgets m_widgets;
    };

    //создание виджета
    template <typename Widget>
    Widget* CSystem<Widget>::create (const std::string &sName)
    {
        std::string sNewWidgetName = sName;

        //такой виджет уже существует
        if (find(sNewWidgetName))
            sNewWidgetName = generateNewWidgetName();

        //создать виджет
        Widget *pWidget = new Widget(sNewWidgetName,*this);
        m_widgets.push_back(pWidget);
        return pWidget;
    }


} //namespace ui
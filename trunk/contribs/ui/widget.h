//widget.h
#pragma once
#include "types.h"
#include <event/ecent.h>
#include <core/NamedObject.h>

namespace ui
{
    //базовый класс для виджета
    class CWidget:
        public event::CSender,
        public core::CNamedObject
    {
    public:
        //addChild
        //removeChild
        //getChildren
        //getParent
        //setParent (protected)

        //... (положение, on***)

        virtual void update (float dt);
        virtual void render ();

    protected:
        //обьектами типа СWidget владеет только CSystem
        //(только он их создает и удаляет)
        //поэтому конструктор и деструктор защищены
        CWidget(std::string sName, PSystem &pSystem);
        virtual ~CWidget();

    private:
        PSystem             m_pSystem;
        std::list<CWidget*> m_children;
        //...

        CWidget (const CWidget&);
        CWidget& operator= (const CWidget&);
    };

} //namespace ui
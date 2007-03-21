//widget.h
#pragma once
#include <list>

namespace ui
{

    //базовый класс для контролов
    class CWidget
    {
    public:
        CWidget (const std::wstring &sName, CUI &rUi);
        virtual ~CWidget();

        const std::wstring& getName() const {return m_sName;}
        CUI&                getUI() {return m_rUI;}
        void                show(bool bVisible);
        bool                isVisible();
        void                bind(CWidget*);
        CWidget*            getParent();
        //...

    private:
        typedef std::list<CWidget*> Widgets;

        Widgets       m_widgets;
        std::wstring  m_sName;
        CUI          &m_rUI;
        CWidget      *m_pParent;

        CWidget(const CWidget&);
        CWidget& operator=(const CWidget&);
    };

} //namespace ui
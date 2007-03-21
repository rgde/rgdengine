//ui.h
#pragma once
#include <boost/noncopyable.hpp>

namespace ui
{

    //UI
    class CUI
    {
    public:
        CUI(PRender render);
       ~CUI();

        PRender* getRender();

        template <typename Widget>
        Widget*  create                    (const std::wstring &sName);
        CWidget* find                      (const std::wstring &sName);
        void     injectMouseMove           (/* ... */);
        void     injectMouseButtonUp       (/* ... */);
        void     injectMouseButtonDown     (/* ... */);
        void     injectMouseButtonDblClick (/* ... */);
        void     injectKeyUp               (/* ... */);
        void     injectKeyDown             (/* ... */);
        void     injectChar                (/* ... */);

        void render(float dt);

    private:
        PRender m_pRender;

        CUI(const CUI&);
        CUI& operator=(const CUI&);
    };

} //namespace ui
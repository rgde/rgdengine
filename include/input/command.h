//command.h
#pragma once
#include "base.h"

namespace input
{

    //команда системы ввода
    class CCommand
    {
    public:
        CCommand(const std::wstring &sName, CInputImpl &rInput):
            m_rInput  (rInput),
            m_sName   (sName),
            m_bLocked (false)
        {
        }

        const std::wstring& getName () const {return m_sName;} //получить 'имя' команды
        void lock     () {m_bLocked = true;}
        void unlock   () {m_bLocked = false;}
        bool islocked () const {return m_bLocked;}

    protected:
        friend class CControl;
        void notifyAllObservers (const CControl &rControl);

    protected:
        friend class CHelper;
        void attachObserver (CHelper *pHelper);
        void detachObserver (CHelper *pHelper);

    private:
        bool                m_bLocked;
        CInputImpl         &m_rInput;
        std::wstring        m_sName;
        std::list<CHelper*> m_helpers;
    };

}
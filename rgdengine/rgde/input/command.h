#pragma once

#include <rgde/input/base.h>

namespace input
{
    //команда системы ввода
    class Command
    {
    public:
        Command(const std::wstring &sName, InputImpl &rInput);

		/// Get command name
        const std::wstring& getName () const {return m_sName;}
        void lock     () {m_bLocked = true;}
        void unlock   () {m_bLocked = false;}
        bool islocked () const {return m_bLocked;}

    protected:
        friend class Control;
        void notifyAllObservers (const Control &rControl);

    protected:
		friend class Helper;
        void attachObserver (Helper *pHelper);
        void detachObserver (Helper *pHelper);

    private:
        bool          m_bLocked;
        InputImpl     &m_rInput;
        std::wstring  m_sName;

		typedef std::list<Helper*> HelpersList;
		typedef HelpersList::iterator HelpersIt;

        HelpersList m_helpers;
    };
}
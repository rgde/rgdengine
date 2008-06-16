#pragma once


namespace input
{
	class InputImpl;
	class Helper;

    //команда системы ввода
    class Command
    {
    public:
        Command(const std::wstring &sName, InputImpl &rInput);

        const std::wstring& getName() const;
        void lock();
        void unlock();
        bool islocked() const;

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

	inline const std::wstring& Command::getName() const 
	{
		return m_sName;
	}

	inline void Command::lock() 
	{
		m_bLocked = true;
	}

	inline void Command::unlock() 
	{
		m_bLocked = false;
	}

	inline bool Command::islocked() const 
	{
		return m_bLocked;
	}
}
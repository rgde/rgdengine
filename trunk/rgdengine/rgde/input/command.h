#pragma once


namespace input
{
	class input_impl;
	class Helper;

    //команда системы ввода
    class Command
    {
    public:
        Command(const std::wstring &sName, input_impl &rInput);

        const std::wstring& get_name() const;
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
        input_impl     &m_rInput;
        std::wstring  m_sName;

		typedef std::list<Helper*> HelpersList;
		typedef HelpersList::iterator HelpersIt;

        HelpersList m_helpers;
    };

	inline const std::wstring& Command::get_name() const 
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
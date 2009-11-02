#pragma once


namespace input
{
	class input_impl;
	class Helper;

    //input system command
    class Command
    {
    public:
        Command(const std::wstring &name, input_impl &input);

        const std::wstring& get_name() const;
        void lock();
        void unlock();
        bool islocked() const;

    protected:
        friend class Control;
        void notify_all (const Control &c);

    protected:
		friend class Helper;
        void attach (Helper *h);
        void detach (Helper *h);

    private:
        bool          m_is_locked;
        input_impl    &m_input;
        std::wstring  m_name;

		typedef std::list<Helper*> helpers_list;
		typedef helpers_list::iterator helpers_iter;

        helpers_list m_helpers;
    };

	inline const std::wstring& Command::get_name() const 
	{
		return m_name;
	}

	inline void Command::lock() 
	{
		m_is_locked = true;
	}

	inline void Command::unlock() 
	{
		m_is_locked = false;
	}

	inline bool Command::islocked() const 
	{
		return m_is_locked;
	}
}
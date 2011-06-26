#pragma once

namespace rgde
{
namespace input
{
	class helper;
	class system;

    //команда системы ввода
    class command
    {
    public:
        command(const std::wstring &name, system &sys);

		const std::wstring& name() const {return m_name;}
		void lock()	{m_locked = true;}
		void unlock() {m_locked = false;}
		bool is_locked() const {return m_locked;}

    protected:
        friend class control;
		// notify all observers
        void notify_all (const control &control);

    protected:
		friend class helper;
        void attach (helper *helper);
        void detach (helper *helper);

    private:
        bool			m_locked;
		std::wstring	m_name;
        system&			m_input;        

		typedef std::list<helper*> helpers_list;
		typedef helpers_list::iterator helpers_iter;

        helpers_list m_helpers;
    };
}
}
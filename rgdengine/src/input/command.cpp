#include "precompiled.h"

#include <rgde/input/command.h>
#include <rgde/input/control.h>
#include <rgde/input/helper.h>

namespace input
{
	Command::Command(const std::wstring &name, input_impl &input):
		m_input  (input),
		m_name   (name),
		m_is_locked (false)
	{
	}

    void Command::notify_all (const Control &c)
    {
        if (m_is_locked)
            return;

        helpers_iter i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            (*i)->notify(c);
            ++i;
        }
    }

    void Command::attach (Helper *h)
    {
        m_helpers.push_back(h);
    }

    void Command::detach (Helper *h)
    {
        helpers_iter i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            if (*i == h)
                i = m_helpers.erase(i);
            else
                ++i;
        }
    }


}
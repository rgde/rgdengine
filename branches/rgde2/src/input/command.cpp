#include "stdafx.h"

#include <rgde/input/command.h>
#include <rgde/input/control.h>
#include <rgde/input/helper.h>

namespace rgde
{
namespace input
{
	command::command(const std::wstring &name, system &sys)
		: m_input  (sys)
		, m_name   (name)
		, m_locked (false)
	{
	}

    void command::notify_all(const control &control)
    {
        if (m_locked)
            return;

        helpers_iter i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            (*i)->notify(control);
            ++i;
        }
    }

    void command::attach (helper *helper)
    {
        m_helpers.push_back(helper);
    }

    void command::detach (helper *helper)
    {
        helpers_iter i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            if (*i == helper)
                i = m_helpers.erase(i);
            else
                ++i;
        }
    }
}
}
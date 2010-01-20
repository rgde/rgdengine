#include "StdAfx.h"
#include "tooltip.h"

#include "system.h"


namespace gui
{

Tooltip::Tooltip(System& sys, const std::string& name) :
	BaseWindow(sys, name),	
	m_fadein(0.f),
	m_fadeout(0.f)
{
}

Tooltip::~Tooltip(void)
{
}

void Tooltip::rise()
{
	if(m_parent)
	{
		ChildrenList& children = m_parent->getChildren();
		ChildrenIter it = std::find_if(children.begin(), children.end(), seeker_(this));
		if(it != children.end())
		{
			children.splice(children.end(), children, it);
		}
	}
}


void Tooltip::reset(void)
{
	setVisible(false);
	setIgnoreInputEvents(true);
}

void Tooltip::show()
{
	rise();
	setVisible(true);
}

}
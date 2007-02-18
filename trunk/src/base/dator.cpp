#include "precompiled.h"
#include "base/dator.h"

namespace base
{
	Dator::Dator(): m_Holder(NULL)
	{
	}

	Dator::Dator(const Dator &v): 
			m_Holder(v.m_Holder ? v.m_Holder->Clone() : NULL)
	{
	}

	Dator::~Dator()
	{
		delete m_Holder;
	}

	Dator& Dator::operator=(const Dator &v)
	{
		delete m_Holder;
		m_Holder = v.m_Holder ? m_Holder->Clone() : NULL;
		return *this;
	}

	bool Dator::Empty() const
	{
		return !m_Holder;
	}

	Dator& Dator::operator=(const std::string &s)
	{
		*m_Holder = s;
		return *this;
	}

	std::string Dator::ToString() const
	{
		return m_Holder->ToString();
	}
}
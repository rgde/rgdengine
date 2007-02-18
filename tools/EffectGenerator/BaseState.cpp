//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 23.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseState.h"

IBaseState::IBaseState(const StateElement& stateElem)
	: m_name(stateElem.getName())
{
}

IBaseState::~IBaseState()
{
}

const std::string& IBaseState::getName() const
{
	return m_name;
}
//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 27.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TurnState.h"

CTurnState::CTurnState(const StateElement& stateElem, uint max)
	: IBaseState(stateElem), IVariableState(stateElem), CArrayState(stateElem, max)
{
	beginNewPass();
}

CTurnState::~CTurnState()
{
}

void CTurnState::setupValue(const std::string& value)
{
	CArrayState::setupValue(m_lastUsed, value);
	m_lastUsed++;
}

void CTurnState::beginNewPass()
{
	reset();

	CArrayState::beginNewPass();
}

void CTurnState::endPass(CPass& pass)
{
	CArrayState::endPass(pass);
}

void CTurnState::reset()
{
	m_lastUsed = 0;
}
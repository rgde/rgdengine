//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 29.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VariableStateValue.h"
#include "Generator.h"

CVariableStateValue* CVariableStateValue::Create(const StateValueElement& stateValue, IVariableState* pState)
{
	if(NULL != pState)
		return new CVariableStateValue(stateValue, pState);
	else
		return NULL;
}

void CVariableStateValue::setup() const
{
	m_pState->setupValue(m_value);
}

CVariableStateValue::CVariableStateValue(const StateValueElement& stateValue, IVariableState* pState)
	: IBaseStateValue(stateValue)
{
	m_pState = pState;
	m_value = TheGenerator::Get().applyConstants(stateValue.getValue());
}

CVariableStateValue::~CVariableStateValue()
{
}
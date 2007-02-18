//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 29.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArrayStateValue.h"
#include "Generator.h"

CArrayStateValue* CArrayStateValue::Create(const StateValueElement& stateValue, IArrayState* pState)
{
	if(NULL != pState)
		return new CArrayStateValue(stateValue, pState);
	else
		return NULL;
}

CArrayStateValue::CArrayStateValue(const StateValueElement& stateValue, IArrayState* pState)
	: IBaseStateValue(stateValue)
{
	m_pState = pState;

	uint index = 0;
	std::string strIndex;

	if(stateValue.findAttribute("n", strIndex))
	{
		TheGenerator::Get().applyConstants(strIndex);
		index = math::Math::abs<int>(base::Lexical_cast<int, std::string>(strIndex));
	}
	else
		base::lwrn<<"Array state \""<<getName()<<"\" doesn't contain index. Using default ("<<index<<").";

	m_value = TheGenerator::Get().applyConstants(stateValue.getValue());
}

CArrayStateValue::~CArrayStateValue()
{
}

void CArrayStateValue::setup() const
{
	m_pState->setupValue(m_index, m_value);
}
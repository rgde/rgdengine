//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 09.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PinPass.h"
#include "Generator.h"
#include "VariableStateValue.h"
#include "ArrayStateValue.h"

CPinPass::CPinPass(const PinPassElement& pinPassElement, const States& states)
{
	m_type = "current";

	if(!pinPassElement.findAttribute("type", m_type))
		base::lwrn<<"PinPass \""<<pinPassElement.getName()<<
		"\" has no information about it's type. Using default (\""<<
		m_type<<"\")...";

	const PinPassElement::LeafElements& children = pinPassElement.getChildren();

	for(size_t i = 0; i < children.size(); i++)
	{
		const StateValueElement& stateValue = children[i];

		IBaseStateValue* pBaseStateValue = NULL;

		std::string name = stateValue.getName();

		std::string strIndex;
		if(stateValue.findAttribute("n", strIndex))
		{
			TheGenerator::Get().applyConstants(strIndex);

			IArrayState* pState = States::findState<IArrayState, States::ArrayStates>(states.arrayStates, name);

			if(NULL != pState)
				pBaseStateValue = CArrayStateValue::Create(stateValue, pState);
			else
				base::lwrn<<"Unknown array state \""<<name<<
							"\". State value \""<<TheGenerator::Get().applyConstants(stateValue.getValue())<<
							"\" will be skipped...";
		}
		else
		{
			IVariableState* pState = States::findState<IVariableState, States::VariableStates>(states.variableStates, name);

			if(NULL != pState)
				pBaseStateValue = CVariableStateValue::Create(stateValue, pState);
			else
				base::lwrn<<"Unknown variable state \""<<name<<
							"\". State value \""<<TheGenerator::Get().applyConstants(stateValue.getValue())<<
							"\" will be skipped...";
		}


		if(NULL != pBaseStateValue)
			m_stateValues.push_back(PStateValue(pBaseStateValue));
	}
}

CPinPass::~CPinPass()
{
}

const std::string& CPinPass::getType() const
{
	return m_type;
}

void CPinPass::setup() const
{
	for(size_t i = 0; i < m_stateValues.size(); i++)
		m_stateValues[i]->setup();
}
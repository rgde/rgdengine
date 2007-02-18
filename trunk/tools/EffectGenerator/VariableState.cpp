//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 26.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VariableState.h"
#include "Generator.h"

CVariableState::CVariableState(const StateElement& stateElem)
	: IBaseState(stateElem), IVariableState(stateElem)
{
	const StateElement::LeafElements& elems = stateElem.getChildren();

	for(size_t i = 0; i < elems.size(); i++)
	{
		const CAttributedLeafElement& elem = elems[i];

		std::string name = elem.getName();

		if(name == "default_value")
			m_defaultValue = TheGenerator::Get().applyConstants(elem.getValue());
		else
			base::lwrn<<"Variable state \""<<getName()<<"\" contains element \""<<
						name<<"\". Nothing know about this type of element. Skipping...";
	}

	beginNewPass();
}

CVariableState::~CVariableState()
{
}

const std::string& CVariableState::getDefaultValue() const
{
	return m_defaultValue;
}

void CVariableState::setupValue(const std::string& value)
{
	m_storedValue = value;
}

void CVariableState::beginNewPass()
{
	setupValue(m_defaultValue);
}

void CVariableState::endPass(CPass& pass)
{
	if(m_storedValue != "")
		pass.addText("      " + getName() + " = " + m_storedValue + ";\n");
}
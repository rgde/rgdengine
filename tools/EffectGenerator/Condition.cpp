//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 10.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Condition.h"
#include "Generator.h"


CCondition::CCondition()
{
}

CCondition::CCondition(const ConditionElement& conditionElement, const CPinPass::States& states)
{
	m_expression = "true";

	if(!conditionElement.findAttribute("expression", m_expression))
		base::lwrn<<"Expression not found in condition. Using default value \""<<
		m_expression<<"\"";


	const ConditionElement::LeafElements& leafs = 
		conditionElement.getChildren();

	for(size_t i = 0; i < leafs.size(); i++)
	{
		std::string name = leafs[i].getName();
		
		if(name == "then")
			readVariation(leafs[i], states, m_then);
		else
		if(name == "else")
			readVariation(leafs[i], states, m_else);
		else
			base::lwrn<<"Unknown element \""<<name<<"\" in if element";
	}
}

CCondition::CCondition(const VariationElement& variationElement, const CPinPass::States& states)
{
	m_expression = "true";

	m_then = CVariation(variationElement, states);
}

CCondition::~CCondition()
{
}

const CVariation& CCondition::getVariation() const
{
	std::string expression = TheGenerator::Get().applyConstants(m_expression);

	if(base::Lexical_cast<bool, std::string>(expression))
		return m_then;
	else
		return m_else;
}

void CCondition::readVariation(const TParentElement<VariationElement>& elem, const CPinPass::States& states, CVariation& variation)
{
	const TParentElement<VariationElement>::LeafElements& leafs = elem.getChildren();

	if(leafs.size() >= 1)
		variation = CVariation(leafs[0], states);
}
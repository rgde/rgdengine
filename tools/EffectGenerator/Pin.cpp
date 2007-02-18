//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 29.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pin.h"


CPin::CPin(const TiXmlElement* pinElem, const CPinPass::States& states)
{
	if(!pinElem)
		return;

	std::string strPriority = "0";

	if(!pinElem->Attribute("priority"))
		base::lwrn<<"Pin element doesn't contain information "<<
		"about it's priority. Usindg default (\""<<strPriority<<
		"\")...";
	else
		strPriority = pinElem->Attribute("priority");

	m_priority = math::Math::abs<int>(base::Lexical_cast<int, std::string>(strPriority));

	const TiXmlElement* first   = pinElem->FirstChildElement();
	const TiXmlElement* current = first;

	while(current != NULL)
	{
		std::string name = current->Value();

		if(name == "variation")
			m_conditions.push_back(CCondition(VariationElement(current), states));
		else
		if(name == "if")
			m_conditions.push_back(CCondition(ConditionElement(current), states));
		else
			base::lwrn<<"Unknown element \""<<name<<
						"\" in pin element.";

		current = current->NextSiblingElement();
	}
}

uint CPin::getPriority() const
{
	return m_priority;
}

bool CPin::operator >(const CPin& pin) const
{
	if(m_priority > pin.getPriority())
		return true;
	else
		return false;
}

const CPin::Conditions& CPin::getConditions() const
{
	return m_conditions;
}
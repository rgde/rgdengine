//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 29.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Condition.h"

class CPin
{
public:

	typedef std::vector<CCondition> Conditions;

public:

	CPin(const TiXmlElement* pinElem,
		 const CPinPass::States& states);

	uint getPriority() const;


	bool operator >(const CPin& pin) const;

	const Conditions& getConditions() const;

private:

	uint       m_priority;
	Conditions m_conditions;

};
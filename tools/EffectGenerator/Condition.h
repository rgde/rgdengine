//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 10.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Variation.h"

typedef TAttributedParentElement<TParentElement<VariationElement> > ConditionElement;

class CCondition
{
public:

	CCondition();
	CCondition(const ConditionElement& conditionElement, const CPinPass::States& states);
	CCondition(const VariationElement& variationElement, const CPinPass::States& states);
	~CCondition();

	const CVariation& getVariation() const;

private:

	void readVariation(const TParentElement<VariationElement>& elem,
		const CPinPass::States& states, CVariation& variation);

private:

	CVariation m_then,
			   m_else;

	std::string m_expression;

};

typedef std::vector<CCondition> Combination;
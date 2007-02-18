//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 30.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "PinPass.h"
#include "Constants.h"


typedef TAttributedParentElement<PinPassElement> VariationElement;

class CVariation
{
public:

	typedef std::vector<CPinPass> PinPasses;

public:

	CVariation();
	CVariation(const VariationElement& variationElement,
			   const CPinPass::States& states);
	~CVariation();

	const CConstants& getConstants() const;

	const PinPasses& getPinPasses() const;


	const std::string& getNameAdd() const;

private:

	CConstants  m_constants;
	PinPasses   m_pinPasses;
	std::string m_nameAdd;

};
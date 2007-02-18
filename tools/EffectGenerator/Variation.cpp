//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 30.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Variation.h"
#include "Generator.h"

CVariation::CVariation()
{
}

CVariation::CVariation(const VariationElement& variationElement, const CPinPass::States& states)
{
	variationElement.findAttribute("name_add", m_nameAdd);

	const VariationElement::LeafElements& children = variationElement.getChildren();

	for(size_t i = 0; i < children.size(); i++)
	{
		const PinPassElement& pinPassElement = children[i];

		IBaseStateValue* pBaseStateValue = NULL;

		std::string name = pinPassElement.getName();

		if(name == "pin_pass")
		{
			CPinPass pinPass(pinPassElement, states);

			m_pinPasses.push_back(pinPass);
		}
		else
		if(name == "constants")
		{
			m_constants = CConstants(pinPassElement);
		}
		else
			base::lwrn<<"Variation \""<<variationElement.getName()<<
			"\" has unknown element \""<<name<<"\". Skipping...";
	}
}

CVariation::~CVariation()
{
}

const CVariation::PinPasses& CVariation::getPinPasses() const
{
	return m_pinPasses;
}

const CConstants& CVariation::getConstants() const
{
	return m_constants;
}

const std::string& CVariation::getNameAdd() const
{
	return m_nameAdd;
}
//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 02.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LeafXMLTypes.h"

//CLeafElement
CLeafElement::CLeafElement(const TiXmlElement* elem)
	: IBaseElement(elem)
{
	if(!elem)
	{
		base::lwrn<<"Can't create leaf, element pointer is invalid.";
		return;
	}
	

	const TiXmlText* text = NULL;
	const TiXmlNode* current = elem->FirstChild();

	while(current != NULL && text == NULL)
	{
		text = current->ToText();
		current = current->NextSibling();
	}

	if(text == NULL)
	{
		base::lwrn<<"Element \""<<getName()<<"\" has to be leaf but isn't"<<
					" text element. Result leaf won't have value...";
	}
	else
		m_value = text->Value();
}

const std::string& CLeafElement::getValue() const
{
	return m_value;
}

//CAttributedLeafElement
CAttributedLeafElement::CAttributedLeafElement(const TiXmlElement* elem)
	: CLeafElement(elem), IHasAttributes(elem)
{
}
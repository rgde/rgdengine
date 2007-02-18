//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 02.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseXMLTypes.h"
#include "Generator.h"

//IBaseElement
IBaseElement::IBaseElement(const TiXmlElement* elem)
{
	if(!elem)
	{
		base::lwrn<<"Can't get element name, element pointer is invalid.";
		return;
	}

	//Value for TiXmlElement is element's name
	m_name = elem->Value();
}

const std::string& IBaseElement::getName() const
{
	return m_name;
}

//IHasAttributes
IHasAttributes::IHasAttributes(const TiXmlElement* elem)
{
	if(!elem)
	{
		base::lwrn<<"Can't read attributes, element pointer is invalid.";
		return;
	}


	const TiXmlAttribute* first   = elem->FirstAttribute();
	const TiXmlAttribute* current = first;

	while(current != NULL)
	{
		m_attributes.insert(Attribute(current->Name(), current->Value()));
		current = current->Next();
	}
}

bool IHasAttributes::findAttribute(const std::string& name, std::string& result) const
{
	Attributes::const_iterator it = m_attributes.find(name);
	if(it == m_attributes.end())
		return false;

	result = it->second;
	return true;
}
//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 02.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseXMLTypes.h"

class CLeafElement : public IBaseElement
{
public:

	CLeafElement(const TiXmlElement* elem);

	const std::string& getValue() const;

private:

	std::string m_value;

};

class CAttributedLeafElement : public CLeafElement, public IHasAttributes
{
public:

	CAttributedLeafElement(const TiXmlElement* elem);

};
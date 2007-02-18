//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 02.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

class IBaseElement
{
public:

	IBaseElement(const TiXmlElement* elem);

	const std::string& getName() const;

private:

	std::string m_name;

};

//First string is Name, second string is Value.
typedef std::map<std::string, std::string> Attributes;
typedef Attributes::value_type             Attribute;

class IHasAttributes
{
public:

	IHasAttributes(const TiXmlElement* elem);

	bool findAttribute(const std::string& name, std::string& result) const;

private:

	Attributes  m_attributes;

};
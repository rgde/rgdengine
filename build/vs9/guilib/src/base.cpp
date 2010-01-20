#include "stdafx.h"
#include "base.h"

NamedObject::NamedObject(const std::string& name) 
	: m_strName(name)
{
}

const std::string& NamedObject::getName() const 
{
	return m_strName;
}

void NamedObject::setName(const std::string& name)
{
	m_strName = name;
}
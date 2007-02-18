//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 05.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseStateValue.h"

IBaseStateValue::IBaseStateValue(const StateValueElement& variationElement)
	: m_name(variationElement.getName())
{
}

IBaseStateValue::~IBaseStateValue()
{
}

const std::string& IBaseStateValue::getName() const
{
	return m_name;
}
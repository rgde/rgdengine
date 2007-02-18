//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 26.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArrayState.h"
#include "Generator.h"
#include "Constants.h"

CArrayState::CArrayState(const StateElement& stateElem, uint max)
	: IBaseState(stateElem), IArrayState(stateElem), m_max(max)
{
	const StateElement::LeafElements& elems = stateElem.getChildren();

	for(size_t i = 0; i < elems.size(); i++)
	{
		const CAttributedLeafElement& elem = elems[i];

		std::string name = elem.getName();

		if(name == "default_value")
		{
			int index = -1;

			std::string strIndex;
			if(elem.findAttribute("n", strIndex))
			{
				TheGenerator::Get().applyConstants(strIndex);
				index = math::Math::abs<int>(base::Lexical_cast<int, std::string>(strIndex));
			}

			std::string value = TheGenerator::Get().applyConstants(elem.getValue());

			if(index >= 0)
				setDefaultValue(index, value);
			else
				m_defaultValue = value;
		}
		else
			base::lwrn<<"Array state \""<<getName()<<"\" contains element \""<<
						name<<"\". Nothing know about this type of element. Skipping...";
	}

	m_storedValues.resize(m_max);

	beginNewPass();
}

CArrayState::~CArrayState()
{
}

uint CArrayState::getMax() const
{
	return m_max;
}

std::string CArrayState::getDefaultValue(uint n) const
{
	std::string result = findDefaultValue(n);
	if(result == "")
		return m_defaultValue;
	else
		return result;
}

void CArrayState::setupValue(uint n, const std::string& value)
{
	if(n >= m_max)
	{
		base::lwrn<<"Trying to setup value \""<<value<<"\" for array state \""<<getName()<<"\""<<
		            " element with index "<<n<<" which is greather then max value ("<<m_max<<
					"). Skipping value...";
		return;
	}

	m_storedValues[n] = CConstants::applyConstant(value,
						CConstants::Constant("n", base::Lexical_cast<std::string, int>(n)));
}

void CArrayState::beginNewPass()
{
	for(uint i = 0; i < m_max; i++)
		setupValue(i, getDefaultValue(i));
}

void CArrayState::endPass(CPass& pass)
{
	for(uint i = 0; i < m_max; i++)
		if(m_storedValues[i] != "")
			pass.addText("      " + getName() + "[" + base::Lexical_cast<std::string, int>(i) + "] = " + m_storedValues[i] + ";\n");
}

void CArrayState::setDefaultValue(uint n, const std::string& value)
{
	if(n >= m_max)
	{
		base::lwrn<<"Trying to set default value \""<<value<<"\" for array state \""<<getName()<<"\""<<
		            " element with index "<<n<<" which is greather then max value ("<<m_max<<
					"). Skipping default value...";
		return;
	}

	m_defaultValues.insert(DefaultValues::value_type(n, value));
}

std::string CArrayState::findDefaultValue(uint n) const
{
	DefaultValues::const_iterator it = m_defaultValues.find(n);

	if(it != m_defaultValues.end())
		return it->second;
	else
		return std::string();
}
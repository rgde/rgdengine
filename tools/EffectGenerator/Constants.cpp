//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 10.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Constants.h"

CConstants::CConstants()
{
}

CConstants::CConstants(const ConstantsElement& constantsElement)
{
	const ConstantsElement::LeafElements& leafs = 
		constantsElement.getChildren();

	for(size_t i = 0; i < leafs.size(); i++)
	{
		const ConstantElement& constElem = leafs[i];

		addConstant(Constant(constElem.getName(), constElem.getValue()));
	}
}

CConstants::~CConstants()
{
}

void CConstants::add(const CConstants& constants)
{
	const Constants& constantsToAdd = constants.getConstants();
	for(ConstantsConstIterator it = constantsToAdd.begin();
		it != constantsToAdd.end(); it++)
		m_constants.insert(*it);
}

void CConstants::reset()
{
	m_constants.clear();
}

bool CConstants::getConstantValue(const std::string& name, std::string& value, bool generateWarning) const
{
	ConstantsConstIterator result;

	if(findConstant(name, result))
	{
		value = result->second;
		return true;
	}
	else
	{
		if(generateWarning)
			base::lwrn<<"Constant \""<<name<<
			"\" not found. Using value \""<<
			value<<"\"...";
		return false;
	}
}

std::string CConstants::getConstantValue(const std::string& name, const std::string& defaultValue, bool generateWarning) const
{
	std::string result = defaultValue;

	getConstantValue(name, result);

	return result;
}

std::string CConstants::apply(const std::string& str) const
{
	std::string result = str;

	apply(result);

	return result;
}

void CConstants::apply(std::string& str) const
{
	apply(str, '%', true);
	apply(str, '!', false);
}

std::string CConstants::applyConstant(const std::string& str, const Constant& constant)
{
	std::string result = str;

	applyConstant(result, constant);

	return result;
}

void CConstants::applyConstant(std::string& str, const Constant& constant)
{
	Constants constants;
	constants.insert(constant);
	CConstants constantsClass(constants);
	constantsClass.apply(str);
}

const CConstants::Constants& CConstants::getConstants() const
{
	return m_constants;
}

CConstants::CConstants(const Constants& constants)
	: m_constants(constants)
{
}

size_t CConstants::cutConstantName(std::string& str, std::string& constName, std::string& constFullName, char openCloseSymbol, size_t firstInput)
{
	size_t first = str.find_first_of(openCloseSymbol, firstInput);
	if(first == -1)
		return -1;
	size_t second = str.find_first_of(openCloseSymbol, first + 1);
	if(second == -1)
	{
		base::lwrn<<"Closing symbol '"<<openCloseSymbol<<
					"' not found while applying constants to string \""<<
					str<<"\". Constant name won't be substituted for contstant value...";
		return -1;
	}
	else

	constName = str.substr(first + 1, second - 1 - first);
	constFullName = str.substr(first, second + 1 - first);
	str.erase(first, second + 1 - first);

	return first;
}

bool CConstants::findConstant(const std::string& name, ConstantsConstIterator& iter) const
{
	Constants::const_iterator it = m_constants.find(name);

	if(it != m_constants.end())
	{
		iter = it;
		return true;
	}
	else
		return false;
}

void CConstants::addConstant(const Constant& constant)
{
	ConstantsConstIterator alreadyExists;

	if(findConstant(constant.first, alreadyExists))
		base::lwrn<<"Constant with name \""<<constant.first<<
		"\" already exists and has value \""<<
		alreadyExists->second<<"\". Redefinition is not allowed";
	else
		m_constants.insert(constant);
}

void CConstants::apply(std::string& str, char openCloseSymbol, bool generateWarning) const
{
	std::string constName, constFullName;
	size_t pos = -1;
	while(-1 != (pos = cutConstantName(str, constName, constFullName, openCloseSymbol, pos + 1 + constFullName.size())))
	{
		ConstantsConstIterator result;
		if(!findConstant(constName, result))
		{
			if(generateWarning)
				base::lwrn<<"Unknown constant \""<<constName<<
							"\". Constant name won't be substituted for contstant value...";
			str.insert(pos, constFullName);
		}
		else
			str.insert(pos, result->second);
	}
}
//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 10.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "XMLTypes.h"

typedef CAttributedLeafElement ConstantElement;
typedef TAttributedParentElement<ConstantElement> ConstantsElement;

class CConstants
{
public:

	//First string is name, second is value
	typedef std::map<std::string, std::string> Constants;
	typedef Constants::value_type              Constant;
	typedef Constants::const_iterator          ConstantsConstIterator;

public:

	CConstants();
	CConstants(const ConstantsElement& constantsElement);
	~CConstants();

	void add(const CConstants& constants);
	void reset();


	bool        getConstantValue(const std::string& name,
								 std::string& value,
								 bool generateWarning = true) const;
	std::string getConstantValue(const std::string& name,
								 const std::string& defaultValue,
								 bool generateWarning = true) const;


	std::string apply(const std::string& str) const;
	void apply(std::string& str) const;


	static std::string applyConstant(const std::string& str,
									 const Constant& constant);
	static void        applyConstant(std::string& str,
									 const Constant& constant);
	
	const Constants& getConstants() const;

private:

	CConstants(const Constants& constants);

	static size_t cutConstantName(std::string& str, std::string& constName,
								  std::string& constFullName,
								  char openCloseSymbol = '%',
								  size_t first = -1);

	bool findConstant(const std::string& name, ConstantsConstIterator& constant) const;

	void addConstant(const Constant& constant);

	void apply(std::string& str, char openCloseSymbol,
			   bool generateWarning) const;

private:

	Constants m_constants;

};
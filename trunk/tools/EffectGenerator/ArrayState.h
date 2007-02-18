//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 26.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseState.h"

class CArrayState : public IArrayState
{
public:

	CArrayState(const StateElement& stateElem, uint max);
	~CArrayState();

	uint getMax() const;

	inline std::string getDefaultValue(uint n) const;


	void setupValue(uint n, const std::string& value);

	void beginNewPass();
	void endPass(CPass& pass);

private:

	void               setDefaultValue(uint n, const std::string& value);
	inline std::string findDefaultValue(uint n) const;

private:

	typedef std::map<uint, std::string> DefaultValues;
	typedef std::vector<std::string>    StoredValues;

private:

	DefaultValues m_defaultValues;
	std::string   m_defaultValue;
	uint          m_max;
	StoredValues  m_storedValues;

};
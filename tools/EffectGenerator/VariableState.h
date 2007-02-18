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

class CVariableState : public IVariableState
{
public:

	CVariableState(const StateElement& stateElem);
	~CVariableState();

	const std::string& getDefaultValue() const;


	void setupValue(const std::string& value);

	void beginNewPass();
	void endPass(CPass& pass);

private:

	std::string m_defaultValue;
	std::string m_storedValue;

};
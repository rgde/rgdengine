//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 29.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseStateValue.h"
#include "BaseState.h"

class CVariableStateValue : public IBaseStateValue
{
public:

	static CVariableStateValue* Create(const StateValueElement& stateValue,
									   IVariableState* pState);
	~CVariableStateValue();



	void setup() const;

private:

	CVariableStateValue(const StateValueElement& stateValue,
						IVariableState* pState);

private:

	IVariableState* m_pState;
	std::string   m_value;

};
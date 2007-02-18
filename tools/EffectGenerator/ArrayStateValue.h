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

class CArrayStateValue : public IBaseStateValue
{
public:

	static CArrayStateValue* Create(const StateValueElement& stateValue,
								    IArrayState* pState);
	~CArrayStateValue();


	void setup() const;

private:

	CArrayStateValue(const StateValueElement& stateValue,
					 IArrayState* pState);

private:

	IArrayState* m_pState;
	std::string  m_value;
	uint         m_index;

};
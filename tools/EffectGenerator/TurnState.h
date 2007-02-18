//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 27.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseState.h"
#include "ArrayState.h"

class CTurnState : public IVariableState,
				   private CArrayState
{
public:

	CTurnState(const StateElement& stateElem, uint max);
	~CTurnState();


	void setupValue(const std::string& value);

	void beginNewPass();
	void endPass(CPass& pass);

private:

	void reset();

private:

	uint m_lastUsed;

};
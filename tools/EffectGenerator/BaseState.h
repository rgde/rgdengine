//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 23.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "XMLTypes.h"
#include "Pass.h"

//To read States of any types easily
typedef TParentElement<CAttributedLeafElement> StateElement;

class IBaseState
{
public:

			IBaseState(const StateElement& stateElem);
	virtual ~IBaseState();

	const std::string& getName() const;

	//Tells state that new pass has begun
	virtual void beginNewPass() = 0;

	//Tells state that pass ended
	virtual void endPass(CPass& pass) = 0;

private:

	std::string m_name;

};

typedef boost::shared_ptr<IBaseState> PState;


class IVariableState : public virtual IBaseState
{
public:

	IVariableState(const StateElement& stateElem)
		: IBaseState(stateElem)
	{
	}

	virtual void setupValue(const std::string& value) = 0;

};

class IArrayState : public virtual IBaseState
{
public:

	IArrayState(const StateElement& stateElem)
		: IBaseState(stateElem)
	{
	}

	virtual void setupValue(uint n, const std::string& value) = 0;

};
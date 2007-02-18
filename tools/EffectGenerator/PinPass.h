//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 09.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "XMLTypes.h"
#include "BaseStateValue.h"
#include "BaseState.h"

typedef TAttributedParentElement<StateValueElement> PinPassElement;

class CPinPass
{
public:

	typedef std::vector<PStateValue> StateValues;

	struct States
	{
		template<class S, class T>
		static S* findState(const T& states, const std::string& name)
		{
			T::const_iterator it = states.find(name);

			if(it == states.end())
				return (S*)(NULL);
			else
				return it->second;
		}

		typedef std::map<std::string, IVariableState*> VariableStates;
		typedef std::map<std::string, IArrayState*>    ArrayStates;

		VariableStates variableStates;
		ArrayStates    arrayStates;
	};

public:

	CPinPass(const PinPassElement& pinPassElement,
			 const States& states);
	~CPinPass();

	const std::string& getType() const;


	void setup() const;

private:

	std::string m_type;
	StateValues m_stateValues;

};
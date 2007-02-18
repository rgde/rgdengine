//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 05.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "XMLTypes.h"

typedef CAttributedLeafElement StateValueElement;

class IBaseStateValue
{
public:

			IBaseStateValue(const StateValueElement& variationElement);
	virtual ~IBaseStateValue();

	const std::string& getName() const;


	virtual void setup() const = 0;

private:

	std::string m_name;

};

typedef boost::shared_ptr<IBaseStateValue> PStateValue;
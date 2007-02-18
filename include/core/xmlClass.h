#pragma once

#include "core/Property.h"
#include "core/NamedObject.h"
#include "core/factory.h"
#include "base/smart_ptr_helpers.h"

namespace core
{
	class XmlClass :  public CPropertyOwner, 
					  public CFunctionsOwner, 
					  public virtual CNamedObject, 
					  public virtual base::CBaseRefCounted
	{
	public:
		XmlClass(const std::string& name) : CNamedObject(name)
		{
			//REGISTER_PROPERTY(Name, std::string)
		}
		virtual ~XmlClass(){}
	};
}
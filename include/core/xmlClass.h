#pragma once

#include "core/Property.h"
#include "core/NamedObject.h"
#include "core/factory.h"
#include "base/smart_ptr_helpers.h"

namespace core
{
	class XmlClass :  public CPropertyOwner, 
					  public CFunctionsOwner, 
					  public virtual NamedObject, 
					  public virtual base::CBaseRefCounted
	{
	public:
		XmlClass(const std::string& name) : NamedObject(name)
		{
			//REGISTER_PROPERTY(Name, std::string)
		}
		virtual ~XmlClass(){}
	};
}
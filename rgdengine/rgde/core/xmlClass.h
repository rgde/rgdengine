#pragma once

#include <rgde/core/Property.h>
#include <rgde/core/NamedObject.h>
#include <rgde/core/factory.h>
#include <rgde/base/smart_ptr_helpers.h>

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
#pragma once

#include <rgde/core/Property.h>
#include <rgde/core/NamedObject.h>
#include <rgde/core/factory.h>
#include <rgde/base/smart_ptr_helpers.h>

namespace core
{
	class XmlClass :  public PropertyOwner, 
					  public FunctionsOwner, 
					  public virtual NamedObject, 
					  public virtual base::BaseRefCounted
	{
	public:
		XmlClass(const std::string& name) : NamedObject(name)
		{
			//REGISTER_PROPERTY(Name, std::string)
		}
		virtual ~XmlClass(){}
	};
}
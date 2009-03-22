#pragma once

#include <rgde/core/property.h>
#include <rgde/core/named_object.h>
#include <rgde/core/factory.h>
#include <rgde/base/smart_ptr_helpers.h>

namespace core
{
	class XmlClass :  public PropertyOwner, 
					  public FunctionsOwner, 
					  public virtual named_object, 
					  public virtual base::refcounted
	{
	public:
		XmlClass(const std::string& name) : named_object(name)
		{
			//REGISTER_PROPERTY(Name, std::string)
		}
		virtual ~XmlClass(){}
	};
}
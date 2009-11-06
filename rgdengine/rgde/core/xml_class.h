#pragma once

#include <rgde/core/property.h>
#include <rgde/core/named_object.h>
#include <rgde/core/factory.h>
#include <rgde/base/smart_ptr_helpers.h>

namespace core
{
	class meta_class :  public named_object, 
						public base::refcounted
						//public property_owner,
						//public functions_owner,
	{
	public:
		meta_class(const std::string& name) : named_object(name)
		{
			//REGISTER_PROPERTY(Name, std::string)
		}
		virtual ~meta_class(){}
	};
}
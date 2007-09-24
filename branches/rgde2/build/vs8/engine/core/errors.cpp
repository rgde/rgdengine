#include "stdafx.h"

#include <engine/core/errors.h>

namespace rgde
{
	namespace core
	{
		namespace errors
		{
			//////////////////////////////////////////////////////////////////////////
			core_exception::core_exception(const std::string& error_message, const std::string& module_name)
				: m_module_name(module_name), m_error_message(error_message)
			{
				m_full_message = "Error: " + error_message + " in module '" + m_module_name + "'";
			}

			const char * core_exception::what() const
			{
				return m_full_message.c_str();
			}

			const char * core_exception::module() const 
			{
				return m_module_name.c_str();
			}

			const char * core_exception::error_message() const
			{
				return m_error_message.c_str();
			}

			//////////////////////////////////////////////////////////////////////////
			not_implemented::not_implemented(const std::string& method)
				: core_exception(std::string("'") + method + "' is not implemented")
			{

			}

			//////////////////////////////////////////////////////////////////////////
		}
	}
}
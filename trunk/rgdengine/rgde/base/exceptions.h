/** @file exceptions.h  
*  Basic error reporting mechanisms.
*
*  3 major ways to report errors:
*  1) throw_<X>
*  2) asserting error codes
*  3) usual error codes (does not need a support here)
*  
*  @author sleepydrago
*  @date june 2005
*/
#pragma once

#include "rgde/base/log.h"
#include "rgde/base/log_helper.h"

namespace base
{
	/** wraps all throw statements to make life/debug/log easier
	*/
	template <class X>
		void throw_(const X& ex)
	{
		///@todo log what and when
		assert(false); /// for debug - we just wanna break - if not comment it out
		throw ex;
	}
	/** base class for project's exceptions.
	*
	*  @note file&line info is for debug builds only and are not expected to be placed manually
	*  @warning normal way to provide file&line info is to use BASE_EXCEPTION_ARGS macro
	*  eg 
	*    class MyEx:public base_exception{ 
	*    public: 
	*        MyEx(const std::string&msg):BASE_EXCEPTION_ARGS1(msg) {} 
	*    };
	*/
	class base_exception : public std::exception
	{
	public:
		base_exception(const std::string& str, unsigned code=0, const char* f="", int l=0)
			: std::exception(str.c_str()),
			m_code(code), m_file(f), m_line(l)
		{
			base::lerr << "base_exception::base_exception() " << str << base::endl;
		}

	protected:
		unsigned m_code;
		const char* m_file;
		const int   m_line;
	};
}
#define BASE_EXCEPTION_ARGS1(what) base_exception((what), 0, __FILE__, __LINE__)
#define BASE_EXCEPTION_ARGS2(what, code) base_exception((what), (code), __FILE__, __LINE__)
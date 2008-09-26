#include "precompiled.h"

#include <rgde/base/log_helper.h>

namespace base
{
	namespace log_internal
	{
		// манипулятор - не выводить время
		log_helper& time_off( log_helper& l )
		{
			l.print_time = false;
			return l;
		}

		log_helper::log_helper(int in_col, int in_style, const char* in_prefix) 
			: print_time(true), col(in_col), style(in_style), prefix(in_prefix) 
		{

		}

		log_helper::~log_helper() 
		{

		}

		log_helper& log_helper::operator << ( log_helper& (*manip)(log_helper&) )
		{
			return (*manip)(*this);
		}
	}

	log_internal::log_helper lmsg(0x81c9aa, LS_ITALIC);
	log_internal::log_helper lnote(0x00FF00, LS_ITALIC, "Note: ");
	log_internal::log_helper lwrn(0xFF00FF, LS_ITALIC, "Warning: ");
	log_internal::log_helper lerr(0xFF0000, LS_ITALIC, "Error: ");
	log_internal::log_helper fatal_error(0xFF0000, LS_ITALIC|LS_BOLD, "Crash: ");
}
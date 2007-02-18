#include "precompiled.h"

#include "base/logHelper.h"

namespace base{


// манипулятор - не выводить время
LogHelper& time_off( LogHelper& l )
{
	l.print_time = false;
	return l;
}

//LogHelper lmsg(0, 0, 0);
//LogHelper lwrn(128, 128, 128);
//LogHelper lerr(255, 0, 0);

LogHelper lmsg(0x81c9aa, LS_ITALIC);
LogHelper lnote(0x00FF00, LS_ITALIC, "Note: ");
LogHelper lwrn(0xFF00FF, LS_ITALIC, "Warning: ");
LogHelper lerr(0xFF0000, LS_ITALIC, "Error: ");
LogHelper fatal_error(0xFF0000, LS_ITALIC|LS_BOLD, "Crash: ");


} // namespace base

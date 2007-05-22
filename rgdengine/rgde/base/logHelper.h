#pragma once

#include "rgde/base/log.h"

namespace base
{

class LogHelper
{
	public:

		LogHelper(int in_col, int in_style = 0, const char* in_prefix = 0) 
			: print_time(true), col(in_col), style(in_style), prefix(in_prefix) {}

		~LogHelper() {}

		template<class T> 
		log& operator << ( T &data )
		{
#ifdef _DEBUG
			log::get().beginLine(col, style);
			if ( !print_time )
			{
				print_time = true;
			}
			else
			{
				log::get() << font(0x336699, LS_BOLD|LS_ITALIC) << "[" << time << "]:: " << efont;
			}

			if ( prefix )
			{
				log::get() << prefix;
			}

			return log::get() << data;
#else
			return log::get();
#endif
		}

		// работа с манипуляторами (время)
		LogHelper& operator << ( LogHelper& (*manip)(LogHelper&) )
		{
			return (*manip)(*this);
		}

		friend LogHelper& time_off( LogHelper& );

	protected:
		// надо ли выводить текущее время
		bool print_time;
		// цвет и стиль
		int col, style;
		const char* prefix;
};


extern LogHelper lmsg;
extern LogHelper lnote;
extern LogHelper lwrn;
extern LogHelper lerr;
extern LogHelper fatal_error;


#define ferr fatal_error << "FATAL ERROR in <a href=" << __FILE__ << ">" << __FILE__ << "</a>" << "(" << __LINE__ << ") : "
}
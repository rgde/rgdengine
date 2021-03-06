#pragma once

#include "rgde/base/log.h"

namespace base
{
namespace log_internal
{
	class log_helper
	{
	public:
		log_helper(int in_col, int in_style = 0, const char* in_prefix = 0);
		~log_helper();

		template<class T> 
		log& operator << ( T &data );

		log_helper& operator << ( log_helper& (*manip)(log_helper&) );

		friend log_helper& time_off( log_helper& );

	protected:
		// need to out current time
		bool print_time;

		int col, style;
		const char* prefix;
	};

	template<class T> 
	log& log_helper::operator << ( T &data )
	{
#ifdef _DEBUG
		log::get().beginLine(col, style);

		if ( print_time )
		{
			log::get() << font(0x336699, log::bold|log::italic) << "[" << time << "]:: " << efont;
		}
		else
		{
			print_time = true;
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
}

extern log_internal::log_helper lmsg;
extern log_internal::log_helper lnote;
extern log_internal::log_helper lwrn;
extern log_internal::log_helper lerr;
extern log_internal::log_helper fatal_error;


#define ferr fatal_error << "FATAL ERROR in <a href=" << __FILE__ << ">" << __FILE__ << "</a>" << "(" << __LINE__ << ") : "
}
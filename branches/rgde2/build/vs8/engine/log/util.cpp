#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <boost/lexical_cast.hpp>

namespace rgde
{
    namespace log
    {
        namespace util
        {
            std::string get_current_time()
            {
                time_t ttime;
                ::time (&ttime);
                struct tm ctime;
                localtime_s (&ctime, &ttime);

                std::string time;
                time.append(boost::lexical_cast<std::string>(ctime.tm_hour));
                time.append(":");
                time.append(boost::lexical_cast<std::string>(ctime.tm_min));
                time.append(":");
                time.append(boost::lexical_cast<std::string>(ctime.tm_sec));

                return time;
            }

            std::string get_current_datetime()
            {
	            SYSTEMTIME t;
	            GetLocalTime (&t);

	            char minute[3], day[3], month[3];
	            sprintf_s (minute, (t.wMinute < 10) ? "0%i" : "%i", t.wMinute);
	            sprintf_s (day, (t.wDay < 10) ? "0%i" : "%i", t.wDay);
	            sprintf_s (month, (t.wMonth < 10) ? "0%i" : "%i", t.wMonth);

	            char time[128];
	            sprintf (time, "%i.%s%_%s.%s.%i", t.wHour, minute, day, month, t.wYear); 
                return std::string(time);
            }
        }
    }
}
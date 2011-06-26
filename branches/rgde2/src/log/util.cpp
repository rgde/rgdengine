#include "stdafx.h"

#include <time.h>

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
	            char minute[3], day[3], month[3];

				time_t ttime;
				::time (&ttime);
				struct tm ctime;
				localtime_s (&ctime, &ttime);

				sprintf_s (minute, (ctime.tm_min < 10) ? "0%i" : "%i", ctime.tm_min);
				sprintf_s (day, (ctime.tm_mday < 10) ? "0%i" : "%i", ctime.tm_mday);
				sprintf_s (month, (ctime.tm_mon < 10) ? "0%i" : "%i", ctime.tm_mon);

	            char time[128];
				sprintf (time, "%i.%s%_%s.%s.%i", ctime.tm_hour, minute, day, month, ctime.tm_year + 1900); 
                return std::string(time);
            }
        }
    }
}
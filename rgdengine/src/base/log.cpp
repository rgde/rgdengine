#include "precompiled.h"

#include "base/log.h"

#include <windows.h>
//-------------------------------------------------------------------------------------------------
namespace base{
//-------------------------------------------------------------------------------------------------
log *log::instance = 0;
//-------------------------------------------------------------------------------------------------
// конструктор
log::log() : initialized( false ), isLineEnded( true )
{
	instance = this;
	init();
}
//-------------------------------------------------------------------------------------------------
// деструктор
log::~log() 
{
	instance = 0;
}
//---------------------------------------------------------------------------------------
std::string getCurrentTime()
{
	SYSTEMTIME t;
	GetLocalTime (&t);

	char day[3], month[3];
	sprintf (day, (t.wDay < 10) ? "0%i" : "%i", t.wDay);
	sprintf (month, (t.wMonth < 10) ? "0%i" : "%i", t.wMonth);

	std::string time;
	time.resize(128);
	sprintf (static_cast<char*> (&time[0]), "%i.%i%_%s.%s.%i", t.wHour, t.wMinute, day, month, t.wYear); 
	return time;
}
//-------------------------------------------------------------------------------------------------
// инициализация
bool log::init()
{
#ifdef _DEBUG
	if ( get().initialized ) return true;

	std::string strLogName;
	strLogName.append("RGDE_Log_");
	strLogName.append(getCurrentTime().c_str());
	strLogName.append(".html");
	get().logFile.open(strLogName.c_str());

	if ( get().logFile.is_open() )
	{
		bool isGood = get().logFile.good();
		get().initialized = true;
		get() << "<html>\n<head>\n";
		get() << "<title>\nRGDE Log!\n</title>\n";
		get() << "\t<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=windows-1251\">\n";
		get() << "</head>\n";
		get() << "<body bgcolor=\"#000000\">\n";
		get() << "<font color=\"#AAAAAA\" face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"2\">\n";
//		get() << "<strong>\n";
		get() << "<b>RGDE Log started " << date << "at " << time << "</b>" << endl;
		return true;
	}
#endif
	return false;
}
//-------------------------------------------------------------------------------------------------
// удаление лога
void log::destroy()
{
	if (0 != instance)
	{
		#ifdef _DEBUG
		endl( get() ); 
//		get() << "</strong>\n";
		get().logFile	<< "</font>\n"
						<< "</body></html>\n";
		if ( get().logFile.is_open() )
		{
			get().logFile.close();
		}
		#endif

		delete instance;
	}
}
//-------------------------------------------------------------------------------------------------
// начало новой строки
log& log::beginLine(byte r, byte g, byte b)
{
#ifdef _DEBUG
	if ( !isLineEnded )
	{
		*this << efont;
		endl( *this );
	}
	logFile << "<font color = \"#";
	logFile.setf(std::ios_base::hex, std::ios_base::basefield);
	logFile.width(2); logFile.fill('0');
	logFile << (int)r;
	logFile.width(2); logFile.fill('0');
	logFile << (int)g;
	logFile.width(2); logFile.fill('0');
	logFile << (int)b << "\">";
	isLineEnded = false;
	logFile.setf(std::ios_base::dec, std::ios_base::basefield);
	return *this;
#endif
	return *this;
}
//-------------------------------------------------------------------------------------------------
// начало новой строки
log& log::beginLine(int col, int style)
{
	if ( !isLineEnded )
	{
		*this << efont;
		endl( *this );
	}
	*this << font(col, style);
	isLineEnded = false;

	return *this;
}
//-------------------------------------------------------------------------------------------------
// запись булевого значения в лог
log& log::operator << ( bool b ) 
{
	if( initialized )
	{
		logFile << ( (b == true) ? "true" : "false" );
	}
	return *this;
}
//-------------------------------------------------------------------------------------------------
// запись символа в лог
log& log::operator << ( char ch )
{
	if( initialized )
		logFile << ch;
	return *this;
}
//-------------------------------------------------------------------------------------------------
// запись int в лог
log& log::operator << ( int i )
{
	if( initialized )
		logFile << i;

	return *this;
}
//---------------------------------------------------------------------------------------
log& log::operator<< (unsigned long l)
{
	if( initialized )
		logFile << l;

	return *this;
}
//-------------------------------------------------------------------------------------------------
// запись size_t в лог
log& log::operator << ( size_t s )
{
	if( initialized )
		logFile << (unsigned int)s;

	return *this;  
}
//-------------------------------------------------------------------------------------------------
// запись float в лог
log& log::operator << ( float f )
{
	if( initialized )
		logFile << f;

	return *this;
}
//-------------------------------------------------------------------------------------------------
// запись double в лог
log& log::operator << ( double d )
{
	if( initialized )
		logFile << d;

	return *this;
}
//-------------------------------------------------------------------------------------------------
// запись строки в лог
log& log::operator << ( const char *str )
{
	if( initialized )
		logFile << str;

	return *this;
}

// запись строки в лог
log& log::operator << ( char *str )
{
	if( initialized )
	{
		logFile << str;
	}
	return *this;
}

// запись строки в лог
log& log::operator << ( std::string str )
{
	if( initialized )
		logFile << str.c_str();

	return *this;  
}
//---------------------------------------------------------------------------------------
log& log::operator << ( wchar_t ch )
{
	if( initialized )
		logFile << ch;

	return *this;
}
//---------------------------------------------------------------------------------------
log& log::operator << ( const wchar_t *str )
{
	if( initialized )
		logFile << str;

	return *this;
}
//---------------------------------------------------------------------------------------
log& log::operator << ( const std::wstring& str )
{
	if( initialized )
		logFile << str.c_str();

	return *this;
}
//-------------------------------------------------------------------------------------------------
// манипулятор - табуляция
log& tab( log& l )
{
	l.get() << "&#160;&#160;&#160;&#160;";
	return l;
}
//-------------------------------------------------------------------------------------------------
// манипулятор - текущая дата
log& date( log& l )
{
	if (l.initialized)
	{
		struct tm *ctime;
		time_t ttime;

		::time( &ttime );
		ctime = localtime( &ttime );

		l.logFile.fill('0'); l.logFile.width( 2 );
		l.logFile << ctime->tm_mday << '.';
		l.logFile.fill('0'); l.logFile.width( 2 );
		l.logFile << ctime->tm_mon << '.';
		l.logFile.fill('0'); l.logFile.width( 2 );
		l.logFile << ctime->tm_year-100 << ' ';
	}
	return l;
}
//-------------------------------------------------------------------------------------------------
// манипулятор - текущее время
log& time( log& l )
{
	if (l.initialized)
	{
		struct tm *ctime;
		time_t ttime;

		::time( &ttime );
		ctime = localtime( &ttime );

		l.logFile.fill('0'); l.logFile.width( 2 );
		l.logFile << ctime->tm_hour << ':';

		l.logFile.fill('0'); l.logFile.width( 2 );
		l.logFile << ctime->tm_min << ':';

		l.logFile.fill('0'); l.logFile.width( 2 );
		l.logFile << ctime->tm_sec;
	}
	return l;
}
//-------------------------------------------------------------------------------------------------
// манипулятор - перевод строки
log& endl( log& l )
{
	if (l.initialized)
	{	
		l.logFile << "<br>" << std::endl;
		l.logFile.flush();
	}
	return l;
}
bool bold = false, italic = false;
//-------------------------------------------------------------------------------------------------
// манипулятор - конец стиля
log& efont( log& l )
{
	if ( bold )
		l << "</b>";

	if ( italic )
		l << "</i>";

	return l << "</font>";
}

log& change_font(log& l, int col, int style)
{
	if (l.initialized)
	{
		
		l << "<font color = \"#";
		l.logFile.setf(std::ios_base::hex, std::ios_base::basefield);

		l.logFile.width(6); l.logFile.fill('0');
		l << col << "\">";
		l.logFile.setf(std::ios_base::dec, std::ios_base::basefield);

		if ( style & LS_BOLD )
		{
			l << "<b>";
			bold = true;
		}

		if ( style & LS_ITALIC )
		{
			l << "<i>";
			italic = true;
		}
	}
	return l;
}

font_manip font(int col, int style)
{
	return font_manip(change_font, col, style);
}
} // namespace base
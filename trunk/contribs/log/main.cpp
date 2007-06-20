#include <Windows.h> //OutputDebugStringA
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>

///////
// 7 //
///////
std::string getCurrentTime()
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

std::string getCurrentDateTime()
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

///////
// 6 //
///////
template <typename CHAR>
struct html_time
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str = "<b>[" + getCurrentTime() + "]</b>&nbsp;";

        std::basic_string<char_type> output(str.begin(), str.end());
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_time<char>,0)
BOOST_IOSTREAMS_PIPABLE(html_time<wchar_t>,0)

template <typename CHAR>
struct text_time
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str("[" + getCurrentTime() + "] ");

        std::basic_string<char_type> output(str.begin(), str.end());
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_time<char>,0)
BOOST_IOSTREAMS_PIPABLE(text_time<wchar_t>,0)

template <typename CHAR>
struct html_msg
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str1("<b>message:</b>&nbsp;<i>");
        std::string str2("</i>");

        std::basic_string<char_type> output(str1.begin(), str1.end());
        std::basic_string<char_type> end(str2.begin(), str2.end());
        output.append(s, s+n);
        output.append(end);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_msg<char>,0)
BOOST_IOSTREAMS_PIPABLE(html_msg<wchar_t>,0)

template <typename CHAR>
struct text_msg
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str("message: ");

        std::basic_string<char_type> output(str.begin(), str.end());
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_msg<char>,0)
BOOST_IOSTREAMS_PIPABLE(text_msg<wchar_t>,0)

template <typename CHAR>
struct html_wrn
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str1("<b>warning:</b>&nbsp;<i>");
        std::string str2("</i>");

        std::basic_string<char_type> output(str1.begin(), str1.end());
        std::basic_string<char_type> end(str2.begin(), str2.end());
        output.append(s, s+n);
        output.append(end);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_wrn<char>,0)
BOOST_IOSTREAMS_PIPABLE(html_wrn<wchar_t>,0)

template <typename CHAR>
struct text_wrn
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str("warning: ");

        std::basic_string<char_type> output(str.begin(), str.end());
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_wrn<char>,0)
BOOST_IOSTREAMS_PIPABLE(text_wrn<wchar_t>,0)

template <typename CHAR>
struct html_err
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str1("<b>error:</b>&nbsp;<i>");
        std::string str2("</i>");

        std::basic_string<char_type> output(str1.begin(), str1.end());
        std::basic_string<char_type> end(str2.begin(), str2.end());
        output.append(s, s+n);
        output.append(end);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_err<char>,0)
BOOST_IOSTREAMS_PIPABLE(html_err<wchar_t>,0)

template <typename CHAR>
struct text_err
{
    typedef CHAR char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string str("error: ");

        std::basic_string<char_type> output(str.begin(), str.end());
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_err<char>,0)
BOOST_IOSTREAMS_PIPABLE(text_err<wchar_t>,0)

///////
// 5 //
///////
class file_wrapper
{
public:
    file_wrapper ()
    {
    }

    file_wrapper (const std::string &filename)
    {
        open(filename);
    }

   ~file_wrapper ()
    {
        close();
    }

    void open (const std::string &filename)
    {
        if (m_file.is_open())
            close();

        m_file.open(filename.c_str());
        m_file << "<html><body>\n";
    }

    void close ()
    {
        if (m_file.is_open())
        {
            m_file << "</body></html>";
            m_file.close();
        }
    }

    //запись в файл multibyte-строк
	std::streamsize write(const char *s, std::streamsize n)
	{
        if (!m_file.is_open())
            return n; //или следует вернуть 0 (хз как буст отреагирует)?

        std::string in(s,n);
        std::string out;
        
        //замена '\n' на "<br/>"
        while (!in.empty())
        {
            std::string::size_type pos = in.find('\n');
            if (pos == std::string::npos)
            {
                out += in;
                in.erase();
            }
            else
            {
                out += in.substr(0, pos);
                out += "</br>\n";
                in = in.substr(pos+1);
            }
        }

        m_file << out;
        return n;
    }

    //запись в файл unicode-строк
	std::streamsize write(const wchar_t *s, std::streamsize n)
	{
        if (!m_file.is_open())
            return n; //или следует вернуть 0 (хз как буст отреагирует)?

        std::wstring in(s,n);
        std::wstring out;
        
        //замена '\n' на "<br/>"
        while (!in.empty())
        {
            std::wstring::size_type pos = in.find(L'\n');
            if (pos == std::wstring::npos)
            {
                out += in;
                in.erase();
            }
            else
            {
                out += in.substr(0, pos);
                out += L"</br>\n";
                in = in.substr(pos+1);
            }
        }

        static char buf[1024];
        wcstombs(buf, out.c_str(), sizeof(buf));

        m_file << std::string(buf, strlen(buf));
        return n;
    }

private:
    file_wrapper (const file_wrapper&);
    file_wrapper& operator= (const file_wrapper&);

    std::ofstream m_file;
};
typedef boost::shared_ptr<file_wrapper> PFileWrapper;

template <typename CHAR>
class ostream_wrapper
{
public:
    typedef CHAR char_type;

    ostream_wrapper ():
        m_out(0)
    {
    }

    ostream_wrapper (std::basic_streambuf<char_type> *sbuf):
        m_out(0)
    {
        open(sbuf);
    }

   ~ostream_wrapper ()
    {
        close();
    }

   void open (std::basic_streambuf<char_type> *sbuf)
    {
        if (m_out.rdbuf())
            close();

        m_out.rdbuf(sbuf);
    }

    void close ()
    {
        if (m_out.rdbuf())
        {
            m_out.rdbuf(0);
        }
    }

	std::streamsize write(const char_type *s, std::streamsize n)
	{
        if (!m_out.rdbuf())
            return n; //или следует вернуть 0 (хз как буст отреагирует)?

        m_out << std::basic_string<char_type>(s, n);
		return n;
	}

private:
    ostream_wrapper (const ostream_wrapper<char_type>&);
    ostream_wrapper& operator= (const ostream_wrapper<char_type>&);

    std::basic_ostream<char_type> m_out;
};
typedef boost::shared_ptr<ostream_wrapper<char> > POStreamWrapperA;
typedef boost::shared_ptr<ostream_wrapper<wchar_t> > POStreamWrapperW;

template <typename CHAR>
class file_dev
{
public:
	typedef CHAR char_type;
	typedef boost::iostreams::sink_tag category;

    file_dev (const file_dev &inst)
    {
        m_pfile = inst.m_pfile;
    }

    file_dev(PFileWrapper pfile)
    {
        m_pfile = pfile;
    }

	std::streamsize write(const char_type *s, std::streamsize n)
	{
		return m_pfile->write(s,n);
	}

    void open (const std::string &filename)
    {
        m_pfile->open(filename);
    }

    void close ()
    {
        m_pfile->close();
    }

private:
    PFileWrapper m_pfile;
};

template <typename CHAR>
class screen_dev
{
public:
	typedef CHAR char_type;
	typedef boost::iostreams::sink_tag category;

    screen_dev(const screen_dev &inst)
    {
        m_postream = inst.m_postream;
    }

    screen_dev(boost::shared_ptr<ostream_wrapper<char_type> > postream)
    {
        m_postream = postream;
    }

	std::streamsize write(const char_type *s, std::streamsize n)
	{
        m_postream->write(s, n);
		return n;
	}

    void open (std::streambuf *sbuf)
    {
        m_postream->open(sbuf);
    }

    void close ()
    {
        m_postream->close();
    }

private:
    boost::shared_ptr<ostream_wrapper<char_type> > m_postream;
};

template <typename CHAR>
class output_dev
{
public:
	typedef CHAR char_type;
	typedef boost::iostreams::sink_tag category;

    //реализаци€ метода write по умолчанию использует OutputDebugStringA
	std::streamsize write(const char_type *s, std::streamsize n)
	{
        std::basic_string<char_type> str(s, n);
        std::string out(str.begin(), str.end());

        OutputDebugStringA(out.c_str());
        return n;
	}
};

//конкретизаци€ char
template <>
class output_dev<char>
{
public:
	typedef char char_type;
	typedef boost::iostreams::sink_tag category;

	std::streamsize write(const char_type *s, std::streamsize n)
	{
        std::string out(s, n);

        OutputDebugStringA(out.c_str());
        return n;
	}
};

//конкретизаци€ wchar_t
template <>
class output_dev<wchar_t>
{
public:
	typedef wchar_t char_type;
	typedef boost::iostreams::sink_tag category;

	std::streamsize write(const char_type *s, std::streamsize n)
	{
        std::basic_string<char_type> out(s, n);

        OutputDebugStringW(out.c_str());
        return n;
	}
};

template <typename CHAR>
class composite_dev
{
public:
	typedef CHAR char_type;
	typedef boost::iostreams::sink_tag category;

    composite_dev (const composite_dev<char_type> &inst):
        m_stream1(inst.m_stream1.rdbuf()),
        m_stream2(inst.m_stream2.rdbuf()),
        m_stream3(inst.m_stream3.rdbuf())
    {
    }

    composite_dev
    (
        std::basic_ostream<char_type> &stream1,
        std::basic_ostream<char_type> &stream2,
        std::basic_ostream<char_type> &stream3
    ):
        m_stream1(stream1.rdbuf()),
        m_stream2(stream2.rdbuf()),
        m_stream3(stream3.rdbuf())
    {
    }

	std::streamsize write(const char_type *s, std::streamsize n)
	{
        std::basic_string<char_type> str(s,n);
        m_stream1 << str;
        m_stream2 << str;
        m_stream3 << str;

        m_stream1.flush();
        m_stream2.flush();
        m_stream3.flush();
		return n;
	}

private:
    std::basic_ostream<char_type> m_stream1;
    std::basic_ostream<char_type> m_stream2;
    std::basic_ostream<char_type> m_stream3;
};

///////
// 4 //
///////
PFileWrapper     pfilewrapper     (new file_wrapper());
POStreamWrapperA postreamwrapperA (new ostream_wrapper<char>());
POStreamWrapperW postreamwrapperW (new ostream_wrapper<wchar_t>());

file_dev  <char> fdev(pfilewrapper);
screen_dev<char> sdev(postreamwrapperA);
output_dev<char> odev;

file_dev  <wchar_t> wfdev(pfilewrapper);
screen_dev<wchar_t> wsdev(postreamwrapperW);
output_dev<wchar_t> wodev;

///////
// 3 //
///////
typedef boost::iostreams::filtering_stream<boost::iostreams::output> dev_stream;

dev_stream lmsg_file   (html_msg<char>() | html_time<char>() | fdev );
dev_stream lmsg_screen (text_msg<char>() | text_time<char>() | sdev );
dev_stream lmsg_output (text_msg<char>() | text_time<char>() | odev );

dev_stream lwrn_file   (html_wrn<char>() | html_time<char>() | fdev );
dev_stream lwrn_screen (text_wrn<char>() | text_time<char>() | sdev );
dev_stream lwrn_output (text_wrn<char>() | text_time<char>() | odev );

dev_stream lerr_file   (html_err<char>() | html_time<char>() | fdev );
dev_stream lerr_screen (text_err<char>() | text_time<char>() | sdev );
dev_stream lerr_output (text_err<char>() | text_time<char>() | odev );

typedef boost::iostreams::wfiltering_stream<boost::iostreams::output> wdev_stream;

wdev_stream wlmsg_file   (html_msg<wchar_t>() | html_time<wchar_t>() | wfdev );
wdev_stream wlmsg_screen (text_msg<wchar_t>() | text_time<wchar_t>() | wsdev );
wdev_stream wlmsg_output (text_msg<wchar_t>() | text_time<wchar_t>() | wodev );

wdev_stream wlwrn_file   (html_wrn<wchar_t>() | html_time<wchar_t>() | wfdev );
wdev_stream wlwrn_screen (text_wrn<wchar_t>() | text_time<wchar_t>() | wsdev );
wdev_stream wlwrn_output (text_wrn<wchar_t>() | text_time<wchar_t>() | wodev );

wdev_stream wlerr_file   (html_err<wchar_t>() | html_time<wchar_t>() | wfdev );
wdev_stream wlerr_screen (text_err<wchar_t>() | text_time<wchar_t>() | wsdev );
wdev_stream wlerr_output (text_err<wchar_t>() | text_time<wchar_t>() | wodev );

///////
// 2 //
///////
typedef boost::iostreams::stream<composite_dev<char> > log_stream;

composite_dev<char> msg_dev(lmsg_file, lmsg_screen, lmsg_output);
composite_dev<char> wrn_dev(lwrn_file, lwrn_screen, lwrn_output);
composite_dev<char> err_dev(lerr_file, lerr_screen, lerr_output);

log_stream lmsg(msg_dev);
log_stream lwrn(wrn_dev);
log_stream lerr(err_dev);

typedef boost::iostreams::stream<composite_dev<wchar_t> > wlog_stream;

composite_dev<wchar_t> wmsg_dev(wlmsg_file, wlmsg_screen, wlmsg_output);
composite_dev<wchar_t> wwrn_dev(wlwrn_file, wlwrn_screen, wlwrn_output);
composite_dev<wchar_t> werr_dev(wlerr_file, wlerr_screen, wlerr_output);

wlog_stream wlmsg(wmsg_dev);
wlog_stream wlwrn(wwrn_dev);
wlog_stream wlerr(werr_dev);

///////
// 1 //
///////

class LogSystem
{
public:
    LogSystem()
    {
        if (counter++ == 0)
            ploghelper = new LogHelper();
    }

   ~LogSystem()
    {
        if (--counter == 0)
            delete ploghelper;
    }

private:
    //гарантировано существование только одного экзмепл€ра этого класса
    class LogHelper
    {
    public:
        LogHelper():
            coutsbuf  (0),
            wcoutsbuf (0),
            cerrsbuf  (0),
            wcerrsbuf (0)
        {
            _wsetlocale(LC_ALL, L"rus");

            pfilewrapper->open(generateLogName());
            postreamwrapperA->open(std::cout.rdbuf());
            postreamwrapperW->open(std::wcout.rdbuf());

            //(w)cout
            coutsbuf = std::cout.rdbuf();
            std::cout.rdbuf(lmsg.rdbuf());

            wcoutsbuf = std::wcout.rdbuf();
            std::wcout.rdbuf(wlmsg.rdbuf());

            //(w)cerr
            cerrsbuf = std::cerr.rdbuf();
            std::cerr.rdbuf(lerr.rdbuf());
            std::cerr.unsetf(std::ios_base::unitbuf);

            wcerrsbuf = std::wcerr.rdbuf();
            std::wcerr.rdbuf(wlerr.rdbuf());
            std::wcerr.unsetf(std::ios_base::unitbuf);
        }

       ~LogHelper()
        {
            //(w)err
            std::wcerr.setf(std::ios_base::unitbuf);
            std::wcerr.rdbuf(wcerrsbuf);
            std::cerr.setf(std::ios_base::unitbuf);
            std::cerr.rdbuf(cerrsbuf);

            //(w)cout
            std::wcout.rdbuf(wcoutsbuf);
            std::cout.rdbuf(coutsbuf);

            postreamwrapperW->close();
            postreamwrapperA->close();
            pfilewrapper->close();
       }

    private:
        //указатели на стандартные потоки
        std::streambuf  *coutsbuf;
        std::wstreambuf *wcoutsbuf;

        std::streambuf  *cerrsbuf;
        std::wstreambuf *wcerrsbuf;

        static std::string generateLogName()
        {
	        std::string strLogName;
	        strLogName.append("RGDE_Log_");
	        strLogName.append(getCurrentDateTime().c_str());
	        strLogName.append(".html");

            return strLogName;
        }

    private:
        LogHelper (const LogHelper&);
        LogHelper& operator= (const LogHelper&);
    };

    static int counter;
    static LogHelper *ploghelper;

private:
    LogSystem (const LogSystem&);
    LogSystem& operator= (const LogSystem&);
};

int LogSystem::counter = 0;
LogSystem::LogHelper* LogSystem::ploghelper = 0;

///////
// 0 //
///////
int main ()
{
/*
    LogSystem log;

    std::cout << "std::cout (LogSystem включена) " << 123 << std::endl;
    std::cerr << "std::cerr (LogSystem включена) " << 123 << std::endl;
    std::wcout << L"std::wcout (LogSystem включена) " << 123 << std::endl;
    std::wcerr << L"std::wcerr (LogSystem включена) " << 123<< std::endl;
/**/
//*
    std::cout << "вывод в std::cout 1" << std::endl;
    std::wcout << L"вывод в std::wcout 1" << std::endl;
    LogSystem log;

    std::cout << "вывод в std::cout 2" << std::endl;
    std::wcout << L"вывод в std::wcout 2" << std::endl;
    {
        LogSystem log;

        std::cout << "вывод в std::cout 3" << std::endl;
        std::wcout << L"вывод в std::wcout 3" << std::endl;
        lmsg << "это тестовое сообщение в лог\n" << 123 << "ups" << std::endl;
        wlmsg << L"это тестовое сообщение в wлог\n" << 123 << L"ups" << std::endl;
        lwrn << "а это - варнинг" << std::endl;
        wlwrn << L"а это - варнинг" << std::endl;
        lerr << "ну и ахтунг (куда же без него)" << std::endl;
        wlerr << L"ну и ахтунг (куда же без него)" << std::endl;
    }

    std::cout << "вывод в std::cout јЅџ–¬јЋ√" << std::endl;
    std::wcout << L"вывод в std::wcout јЅџ–¬јЋ√" << std::endl;
/**/
}
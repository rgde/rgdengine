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

	char day[3], month[3];
	sprintf_s (day, (t.wDay < 10) ? "0%i" : "%i", t.wDay);
	sprintf_s (month, (t.wMonth < 10) ? "0%i" : "%i", t.wMonth);

	char time[128];
	sprintf (time, "%i.%i%_%s.%s.%i", t.wHour, t.wMinute, day, month, t.wYear); 
    return std::string(time);
}

///////
// 6 //
///////
struct html_time
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("<b>[" + getCurrentTime() + "]</b>&nbsp;");
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_time,0)

struct text_time
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("[" + getCurrentTime() + "] ");
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_time,0)

struct html_msg
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("<b>message:</b>&nbsp;<i>");
        output.append(s, s+n);
        output.append("</i>", 4);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_msg,0)

struct text_msg
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("message: ");
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_msg,0)

struct html_wrn
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("<b>warning:</b>&nbsp;<i>");
        output.append(s, s+n);
        output.append("</i>", 4);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_wrn,0)

struct text_wrn
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("warning: ");
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_wrn,0)

struct html_err
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("<b>error:</b>&nbsp;<i>");
        output.append(s, s+n);
        output.append("</i>", 4);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(html_err,0)

struct text_err
{
    typedef char char_type;
    typedef boost::iostreams::multichar_output_filter_tag category;

    template<typename Sink>
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n)
    {
        std::string output("error: ");
        output.append(s, s+n);

        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n;
    }
};
BOOST_IOSTREAMS_PIPABLE(text_err,0)

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
                out += "<br/>\n";
                in = in.substr(pos+1);
            }
        }

        m_file << out;
		return n;
	}

private:
    file_wrapper (const file_wrapper&);
    file_wrapper& operator= (const file_wrapper&);

    std::ofstream m_file;
};
typedef boost::shared_ptr<file_wrapper> PFileWrapper;

class ostream_wrapper
{
public:
    ostream_wrapper ():
        m_out(0)
    {
    }

    ostream_wrapper (std::streambuf *sbuf):
        m_out(0)
    {
        open(sbuf);
    }

   ~ostream_wrapper ()
    {
        close();
    }

   void open (std::streambuf *sbuf)
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

	std::streamsize write(const char *s, std::streamsize n)
	{
        if (!m_out.rdbuf())
            return n; //или следует вернуть 0 (хз как буст отреагирует)?

        m_out << std::string(s, n);
		return n;
	}

private:
    ostream_wrapper (const ostream_wrapper&);
    ostream_wrapper& operator= (const ostream_wrapper&);

    std::ostream m_out;
};
typedef boost::shared_ptr<ostream_wrapper> POStreamWrapper;

class file_dev
{
public:
	typedef char char_type;
	typedef boost::iostreams::sink_tag category;

    file_dev (const file_dev &inst)
    {
        m_pfile = inst.m_pfile;
    }

    file_dev(PFileWrapper pfile)
    {
        m_pfile = pfile;
    }

	std::streamsize write(const char *s, std::streamsize n)
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

class screen_dev
{
public:
	typedef char char_type;
	typedef boost::iostreams::sink_tag category;

    screen_dev(const screen_dev &inst)
    {
        m_postream = inst.m_postream;
    }

    screen_dev(POStreamWrapper postream)
    {
        m_postream = postream;
    }

	std::streamsize write(const char *s, std::streamsize n)
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
    POStreamWrapper m_postream;
};

class output_dev
{
public:
	typedef char char_type;
	typedef boost::iostreams::sink_tag category;

	std::streamsize write(const char *s, std::streamsize n)
	{
        std::string out(s,n);
        OutputDebugStringA(out.c_str());
        return n;
	}
};

class composite_dev
{
public:
	typedef char char_type;
	typedef boost::iostreams::sink_tag category;

    composite_dev (const composite_dev &inst):
        m_stream1(inst.m_stream1.rdbuf()),
        m_stream2(inst.m_stream2.rdbuf()),
        m_stream3(inst.m_stream3.rdbuf())
    {
    }

    composite_dev(std::ostream &stream1, std::ostream &stream2, std::ostream &stream3):
        m_stream1(stream1.rdbuf()),
        m_stream2(stream2.rdbuf()),
        m_stream3(stream3.rdbuf())
    {
    }

	std::streamsize write(const char *s, std::streamsize n)
	{
        std::string str(s,n);
        m_stream1 << str;
        m_stream2 << str;
        m_stream3 << str;

        m_stream1.flush();
        m_stream2.flush();
        m_stream3.flush();
		return n;
	}

private:
    std::ostream m_stream1;
    std::ostream m_stream2;
    std::ostream m_stream3;
};

///////
// 4 //
///////
PFileWrapper    pfilewrapper   (new file_wrapper());
POStreamWrapper postreamwrapper (new ostream_wrapper());

file_dev   fdev(pfilewrapper);
screen_dev sdev(postreamwrapper);
output_dev odev;

///////
// 3 //
///////
typedef boost::iostreams::filtering_stream<boost::iostreams::output> dev_stream;

dev_stream lmsg_file   (html_msg() | html_time() | fdev );
dev_stream lmsg_screen (text_msg() | text_time() | sdev );
dev_stream lmsg_output (text_msg() | text_time() | odev );

dev_stream lwrn_file   (html_wrn() | html_time() | fdev );
dev_stream lwrn_screen (text_wrn() | text_time() | sdev );
dev_stream lwrn_output (text_wrn() | text_time() | odev );

dev_stream lerr_file   (html_err() | html_time() | fdev );
dev_stream lerr_screen (text_err() | text_time() | sdev );
dev_stream lerr_output (text_err() | text_time() | odev );

///////
// 2 //
///////
typedef boost::iostreams::stream<composite_dev> log_stream;

composite_dev msg_dev(lmsg_file, lmsg_screen, lmsg_output);
composite_dev wrn_dev(lwrn_file, lwrn_screen, lwrn_output);
composite_dev err_dev(lerr_file, lerr_screen, lerr_output);

log_stream lmsg(msg_dev);
log_stream lwrn(wrn_dev);
log_stream lerr(err_dev);

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
        LogHelper(): coutsbuf(0)
        {
            pfilewrapper->open(generateLogName());
            postreamwrapper->open(std::cout.rdbuf());

            coutsbuf = std::cout.rdbuf();
            std::cout.rdbuf(lmsg.rdbuf());
        }

       ~LogHelper()
        {
            pfilewrapper->close();
            postreamwrapper->close();

            std::cout.rdbuf(coutsbuf);
        }

    private:
        std::streambuf *coutsbuf;

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
    std::cout << "вывод в std::cout 1" << std::endl;
    LogSystem log;

    std::cout << "вывод в std::cout 2" << std::endl;
    {
        LogSystem log;

        std::cout << "вывод в std::cout 3" << std::endl;
        lmsg << "это тестовое сообщение в лог\n" << 123 << "ups" << std::endl;
        lwrn << "а это - варнинг" << std::endl;
        lerr << "ну и ахтунг (куда же без него)" << std::endl;
        return 0;
    }
}
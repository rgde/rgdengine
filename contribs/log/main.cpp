#include <Windows.h> //OutputDebugStringA 
#include <iostream> 
#include <fstream> 
#include <boost/iostreams/stream.hpp> 
#include <boost/iostreams/stream_buffer.hpp> 
#include <boost/iostreams/categories.hpp> 
#include <boost/iostreams/detail/ios.hpp> 
#include <boost/iostreams/filtering_stream.hpp> 

/////////////////
// три девайса //
/////////////////
//файл
class file_device
{
public: 

    typedef char char_type; 
    typedef boost::iostreams::sink_tag category; 

    file_device(const std::string &filename): 
        m_file(filename.c_str())
    {
    }

    std::streamsize write(const char *s, std::streamsize n) 
    {
        m_file << std::string(s,n);
        return n; 
    }

private: 
    std::ofstream m_file;
};

//консоль
class screen_device 
{
public: 

    typedef char char_type; 
    typedef boost::iostreams::sink_tag category; 

    screen_device(std::streambuf *screen_ostream):
        m_screen(screen_ostream)
    {
    }

    std::streamsize write(const char *s, std::streamsize n) 
    {
        m_screen << std::string(s, n);
        return n; 
    }

private: 
    std::ostream m_screen;
};

//оутпут
class output_device 
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

//по четыре фильтра на каждый девайс
struct time_screen_filter 
{
    typedef char char_type; 
    typedef boost::iostreams::multichar_output_filter_tag category; 

    template<typename Sink> 
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n) 
    {
        std::string output("[12-34-56] "); 
        output.append(s, s+n);
        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n; 
    }
};
BOOST_IOSTREAMS_PIPABLE(time_screen_filter, 0)

struct msg_screen_filter 
{
    typedef char char_type; 
    typedef boost::iostreams::multichar_output_filter_tag category; 

    template<typename Sink> 
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n) 
    {
        boost::iostreams::write(snk, s, n);
        return n; 
    }
};
BOOST_IOSTREAMS_PIPABLE(msg_screen_filter, 0)

struct wrn_screen_filter 
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
BOOST_IOSTREAMS_PIPABLE(wrn_screen_filter, 0)

struct err_screen_filter 
{
    typedef char char_type; 
    typedef boost::iostreams::multichar_output_filter_tag category; 

    template<typename Sink> 
    std::streamsize write(Sink &snk, const char_type *s, std::streamsize n) 
    {
        std::string output("ERROR: "); 
        output.append(s, s+n);
        boost::iostreams::write(snk, output.c_str(), (std::streamsize)output.size());
        return n; 
    }
};
BOOST_IOSTREAMS_PIPABLE(err_screen_filter, 0)

//еще нужны фильтры для файла и оутпута (для оутпута фильтры скорее всего совпадут с экранными)
//...

//тест
int main () 
{
    //screen_device dev(std::cout.rdbuf()); 
    //dev.write("123",2); 

    //output_device dev; 
    //while (1) 
    //{ 
    // Sleep(100); 
    // dev.write("test\n",5); 
    //} 

    //output_device dev; 
    //typedef boost::iostreams::filtering_stream<boost::iostreams::output> log_stream; 
    //log_stream log(time_screen_filter() | dev);
    //log << "hello!\n"; 
    //log.flush();
    //log << "test" << std::endl << "a\nb" << std::endl; 

    file_device   dev1("test.txt");
    //screen_device dev2;
    output_device dev3;

    typedef boost::iostreams::filtering_stream<boost::iostreams::output> log_stream;
    log_stream lmsg(msg_screen_filter() | time_screen_filter() | dev3);
    log_stream lwrn(wrn_screen_filter() | time_screen_filter() | dev3);
    log_stream lerr(err_screen_filter() | time_screen_filter() | dev3);

    //а хочетя что-то типа
    //log_stream lmsg
    //           (
    //             msg_file_filter()   | time_file_filter()   | dev1,
    //             msg_screen_filter() | time_screen_filter() | dev2,
    //             msg_output_filter() | time_output_filter() | dev3,
    //           );

    lmsg << "message 1" << std::endl;
    lmsg << "message 2" << std::endl;

    lwrn << "warning 1" << std::endl;
    lwrn << "warning 2" << std::endl;

    lerr << "error 1" << std::endl;
    lerr << "error 2" << std::endl;
    //... 
    return 0; 
}

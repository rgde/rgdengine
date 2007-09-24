#include "stdafx.h"

#include <engine/core/core.h>

#include <iterator>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/detail/ios.hpp> 
#include <boost/iostreams/filtering_stream.hpp>

namespace io = boost::iostreams;

template<typename Ch>
struct	basic_time_tracer 
{
	typedef	Ch	char_type;
	typedef io::multichar_output_filter_tag	category;

	template<typename Sink>
	std::streamsize write(Sink& snk, const char_type* s, std::streamsize n)
	{
		//using	namespace	boost::posix_time;
		//ptime	time_stamp(second_clock::local_time());
		//std::string	output(to_simple_string(time_stamp));
		std::string	output("21-58-24");
		output.append(": ", 2);
		output.append(s, s+n);

		typedef std::basic_string<char_type> _string;
		_string out(output.begin(), output.end());
		io::write(snk, out.c_str(), (std::streamsize)out.size());
		return	n;
	}			
};

BOOST_IOSTREAMS_PIPABLE(basic_time_tracer, 1)
typedef	basic_time_tracer<char>		time_tracer;
typedef	basic_time_tracer<wchar_t>	wtime_tracer;


class screen_log_device
{
public:
	typedef wchar_t char_type;
	typedef char_type type;
	typedef boost::iostreams::sink_tag category;

	screen_log_device(std::wstring& out_string)
		: m_string(out_string){}

	std::streamsize write(const wchar_t* s, std::streamsize n)
	{
		std::wstring out(s, n);
		m_string += out;
		std::wcout << out.c_str();
		return n;
	}
private:
	std::wstring& m_string;
};


namespace test
{
	struct resource
	{
		resource(const std::wstring& name)
		{
			std::wcout << "resource() " << name.c_str() << std::endl;
		}

		~resource()
		{
			std::wcout << "~resource()" << std::endl;
		}
	};

	typedef boost::shared_ptr<resource> resource_ptr;

	resource_ptr create_resource(const std::wstring& name)
	{
		return resource_ptr(new resource(name));
	}
}




int _tmain(int argc, wchar_t* argv[])
{
	using namespace rgde;
	using namespace std;

	std::wstring log_text;

	typedef boost::iostreams::stream_buffer<screen_log_device>  my_streambuf;
	screen_log_device dev(log_text);
	typedef io::wfiltering_stream<io::output> my_ostream;
	my_ostream log(wtime_tracer() | dev);

	std::wstreambuf *old_buff = wcout.rdbuf();
	//wcout.rdbuf(log.rdbuf());

	log << L"Test" << endl;
	{
		core::utils::timer timer;
		Sleep(1000);
		float elapsed = timer.get_elapsed_time();
		log << L"elapsed time: " << elapsed << endl;
		//new char[100];
		core::log::utils::output_debug_string(L"DEBUG LOG TEST MSG");
	}

	core::utils::get_allocated_mem();

	{	// math test
		math::vec3f v1(1, 2, 3);
		math::vec3f v2(2, 1, 0);
		math::vec3f v3 = v1 + v2;
		//cout << v3 << endl;
	}

	{
		core::windows::window window(L"TestWindow");

		window.show();
		window.update();

		MSG msg = {0};
		//while( window.is_created() )
		//{
		//	if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		//	{
		//		TranslateMessage( &msg );
		//		DispatchMessage( &msg );
		//	}
		//}
	}

	try
	{
		//throw core::errors::not_implemented("actor::clone");
		using namespace test;
		typedef core::utils::resouce_manager<resource> resouce_manager;

		resouce_manager manager(boost::bind<resource_ptr>(&create_resource, _1));

		resource_ptr res1 = manager.get(L"file1.dat");
		resource_ptr res2 = manager.get(L"file2.dat");
	}
	//catch(core::errors::not_implemented& ex)
	//{

	//}
	catch(core::errors::core_exception& ex)
	{
		cout << ex.what() << endl;
	}

	log.flush();

	wcout.rdbuf(old_buff);
	wcout << log_text.c_str();
	wcout.flush();
	
	return 0;
}
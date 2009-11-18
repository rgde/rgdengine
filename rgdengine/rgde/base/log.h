#pragma once

#include "rgde/math/types3d.h"

namespace base
{
	typedef unsigned char uchar;

	class log;
	struct int_manip
	{
		typedef log& (*log_func)(log&, int);

		int_manip(log_func in_func, int in_i) 
			: func(in_func)
			, i(in_i)
		{}

		log_func func;
		int i;
	};

	struct font_manip
	{
		typedef log& (*log_func)(log&, int, int);

		font_manip(log_func in_func, int in_col, int in_style) 
			: func(in_func)
			, col(in_col)
			, style(in_style) 
		{}

		log_func func;
		int col, style;
	};

	class log
	{
	public:
		enum style
		{
			bold	= 1 << 0,
			italic	= 1 << 1,
		};

		static bool init();
		static void destroy();
		static log& get();

		log& beginLine(uchar r, uchar g, uchar b);
		log& beginLine(int col, int style = 0);

		bool isInited() const { return initialized; }

		log& operator << ( bool b );
		log& operator << ( char ch );  
		log& operator << ( int i );  
		log& operator << ( size_t s ); 
		log& operator << ( float ch );
		log& operator << ( double ch );
		log& operator << ( const char *str );
		log& operator << ( char *str );
		log& operator << ( std::string str );
		log& operator << ( wchar_t ch );
		log& operator << ( const wchar_t *str );
		log& operator << ( const std::wstring& str );
		log& operator << (unsigned long l);
		log& operator << ( log& (*manip)(log&) );
		log& operator << ( int_manip &manip );
		log& operator << ( font_manip &manip );

		friend log& endl( log& );
		friend log& date( log& );
		friend log& time( log& );

	protected:
		log();
		~log();

		friend log& print_hex(log& l, int i);
		friend log& change_font(log& l, int col, int style);

	protected:
		// class instance
		static log *instance;

		// log file stream
		std::ofstream logFile;

		// is init was done?
		bool initialized;
		// is line ended?
		bool isLineEnded;
	};

	log& tab( log& );
	log& efont( log& );

	int_manip hex(int i);
	font_manip font(int col, int style = 0);

	//-----------------------------------------------------------------------------------
	log& operator << ( log& l, math::vec3f& v );
	log& operator << ( log& l, math::vec2f& v );
	log& operator << ( log& l, math::vec4f& v );
	log& operator << (log& l, math::Rect& r);

	template <class T> 
	log& operator << ( log& l, std::vector<T>& v )
	{
		unsigned size = (unsigned)v.size();
		l << "std::vector(size: "<< size;
		if( v.empty() )
		{
			l << ")";
			return l;
		}

		l << ", elements: ";
		
		unsigned i = 0;
		for (; i < size-1; ++i)
		{
			l << "v[" << i << "] = " << v[i] << ", ";
		}

		l << "v[" << i << "] = " << v[i] << ")";
		return l;
	}
	//-----------------------------------------------------------------------------------

}

#include "log_helper.h"
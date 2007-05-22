#pragma once

#include "rgde/math/mathTypes3d.h"

namespace base
{

	typedef unsigned char byte;

	class log;
	struct int_manip
	{
		int_manip(log& (*in_func)(log&, int), int in_i) : func(in_func), i(in_i) {}


		log& (*func)(log&, int);
		int i;
	};

	enum LogStyle
	{
		LS_BOLD		= 1 << 0,
		LS_ITALIC	= 1 << 1,
	};

	struct font_manip
	{
		font_manip(log& (*in_func)(log&, int, int), int in_col, int in_style) 
			: func(in_func), col(in_col), style(in_style) {}


			log& (*func)(log&, int, int);
			int col, style;
	};

	class log
	{
	public:
		static bool init();
		static void destroy();

		// получение экземпляра класса лог
		static log& get()
		{
			if ( 0 == instance )
				new log();

			return *instance;
		}

		// начало новой строки
		log& beginLine(byte r, byte g, byte b);

		// начало новой строки
		log& beginLine(int col, int style = 0);

		// инициализирован лог или нет
		bool isInit() const		{ return initialized; }

		// запись булевого значения в лог
		log& operator << ( bool b );

		// запись символа в лог
		log& operator << ( char ch );  

		// запись int в лог
		log& operator << ( int i );  

		// запись size_t в лог
		log& operator << ( size_t s );  

		// запись float в лог
		log& operator << ( float ch );  

		// запись double в лог
		log& operator << ( double ch );  

		// запись строки в лог
		log& operator << ( const char *str );


		// запись строки в лог
		log& operator << ( char *str );

		// запись строки в лог
		log& operator << ( std::string str );


		log& operator << ( wchar_t ch );  
		log& operator << ( const wchar_t *str );
		log& operator << ( const std::wstring& str );

		log& operator<< (unsigned long l);

		// работа с манипуляторами (перевод строки, табуляция итд)
		log& operator << ( log& (*manip)(log&) )
		{
			return (*manip)(*this);
		}

		// работа с манипуляторами, получающими в качестве параметра int (hex)
		log& operator << ( int_manip &manip )
		{
			return manip.func(*this, manip.i);
		}

		// работа с манипулятором стиля
		log& operator << ( font_manip &manip )
		{
			return manip.func(*this, manip.col, manip.style);
		}

		friend log& endl( log& );
		friend log& date( log& );
		friend log& time( log& );

		friend log& print_hex(log& l, int i)
		{
			l.logFile.setf(std::ios_base::hex, std::ios_base::basefield);
			l.logFile.setf(std::ios_base::uppercase);
			l << "0x" << i;
			l.logFile.setf(std::ios_base::dec, std::ios_base::basefield);
			return l;
		}
		friend log& change_font(log& l, int col, int style);

	protected:
		log();
		~log();

	protected:

		// экземпляр класса
		static log *instance;

		// файл
		std::ofstream logFile;

		// успешно ли прошла инициализация
		bool initialized;
		// была ли закончена строка
		bool isLineEnded;
	};


	log& tab( log& );
	log& efont( log& );


	inline int_manip hex(int i)
	{
		return int_manip(print_hex, i);
	}

	font_manip font(int col, int style = 0);

	//-----------------------------------------------------------------------------------
	inline log& operator << ( log& l, math::Vec3f& v )
	{
		l << "(" << v[0] << "," << v[1] << "," << v[2] << ")\n";
		return l;
	}

	//-----------------------------------------------------------------------------------
	inline log& operator << ( log& l, math::Vec2f& v )
	{
		l << "(" << v[0] << "," << v[1] << ")\n";
		return l;
	}

	//-----------------------------------------------------------------------------------
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
		// Neonic: 'unsigned i' moved from cycle scopes
		unsigned i;
		for ( i = 0; i < size-1; ++i)
		{
			l << "v[" << i << "] = " << v[i] << ", ";
		}

		l << "v[" << i << "] = " << v[i] << ")";
		return l;
	}

	//-----------------------------------------------------------------------------------
	inline log& operator << ( log& l, math::Vec4f& v )
	{
		l << "(" << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << ")\n";
		return l;
	}

	//-----------------------------------------------------------------------------------
	inline log& operator << (log& l, math::Rect& r)
	{
		l << "(" << r.x << "," << r.y << "," << r.w << "," << r.h << ")";
		return l;
	}
}

#include "logHelper.h"
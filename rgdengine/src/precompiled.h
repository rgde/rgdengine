#pragma once

//для WM_MOUSEWHELL
#define _WIN32_WINNT 0x0400

//STD
#include <memory>
#include <string>
#include <exception>
#include <cassert>
//#include <math.h>
#include <time.h>

//STL
#include <vector>
#include <list>
#include <map>
#include <iomanip>
#include <algorithm>
#include <strstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

#undef min
#undef max

#include <gmtl/gmtl.h>

namespace math
{
	//using namespace math;
}

#define TIXML_USE_STL
#include "Tinyxml\\tinyxml.h"

//Boost
//#include <boost\\timer.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/tokenizer.hpp>
#include <boost/ref.hpp>
#define BOOST_FUNCTION_MAX_ARGS 3
#include <boost/function.hpp>
#include <boost/bind/apply.hpp>
#include <boost/call_traits.hpp>

#define ASSERT _ASSERTE

typedef unsigned char uchar;
typedef	uchar		  byte;
typedef unsigned int  uint;
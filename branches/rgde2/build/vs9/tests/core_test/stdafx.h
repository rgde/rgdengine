// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define _SCL_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>

#include <map>
#include <vector>
#include <list>
#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/pipeline.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>
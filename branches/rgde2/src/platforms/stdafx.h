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

#include <stdio.h>
#include <tchar.h>

#define COM_NO_WINDOWS_H
#include <objbase.h>

#include <map>
#include <list>
#include <vector>

// TODO: reference additional headers your program requires here
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

template<typename T>
void safe_release(T*& p)
{
	if (NULL == p) return;

	p->Release();
	p = NULL;
}


#include <rgde/core/types.h>
#include <rgde/core/math.h>

#include <rgde/render/device.h>
#include <rgde/render/font.h>
#include <rgde/render/resource.h>
#include <rgde/render/buffer.h>
#include <rgde/render/surface.h>

#include <d3dx9.h>

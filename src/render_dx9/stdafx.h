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

// TODO: reference additional headers your program requires here
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

template<typename T>
void safe_release(T*& p)
{
	if (NULL == p) return;

	p->Release();
	p = NULL;
}


#include <engine/core/types.h>
#include <engine/core/math.h>

#include <engine/render/device.h>
#include <engine/render/font.h>
#include <engine/render/resource.h>
#include <engine/render/buffer.h>
#include <engine/render/surface.h>

#include <d3dx9.h>

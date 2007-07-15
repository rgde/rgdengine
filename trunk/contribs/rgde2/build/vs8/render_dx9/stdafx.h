// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers



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

#include <d3dx9.h>
#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
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

//#define _SECURE_SCL 0
#define _SCL_SECURE_NO_WARNINGS

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#include <dsound.h>
#include <dxerr.h>
#include <d3dx9math.h>

#define ASSERT _ASSERTE

#define COM_NO_WINDOWS_H
#include <objbase.h>

#include <vorbis\vorbisfile.h>

#include <math.h>

#include <map>
#include <vector>
#include <string>
#include <list>
#include <exception>

#include <boost/smart_ptr.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
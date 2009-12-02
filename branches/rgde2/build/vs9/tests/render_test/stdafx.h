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

#include <stdio.h>
#include <tchar.h>


#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#include <dsound.h>
#include <dxerr.h>
#include <d3dx9math.h>

#include <map>
#include <vector>
#include <string>
#include <list>
#include <exception>
#include <iostream>
#include <algorithm>

#include <boost/smart_ptr.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>
#include <boost/timer.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <rgde/core/core.h>
#include <rgde/core/xml/xml.h>
#include <rgde/render/render.h>

#include <windows.h>
#define ASSERT _ASSERTE

#define COM_NO_WINDOWS_H
#include <objbase.h>

#include <vorbis\vorbisfile.h>
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


// RGDE includes
#include <engine/core/core.h>
#include <engine/core/types.h>
#include <engine/render/render.h>

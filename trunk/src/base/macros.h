#pragma warning(disable:4786) // truncation to '255' characters
#ifndef HL_MACROS_H
#define HL_MACROS_H
#include "exception.h"

#define check(expr)			guard(expr) expr unguard
#define advisable(expr)		try{expr}catch(...){}
#endif
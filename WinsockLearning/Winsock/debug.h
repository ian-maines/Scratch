#pragma once
// Copyright Ian Maines
// 04/01/2016
// Reroutable debugging code.

#include "globals.h"

#define WIN32_LEAN_AND_MEAN
#include "afx.h"

#include <string>
#include <iostream>

namespace debug
{
inline void debug_out (std::wstring str)
	{
	TRACE (str.c_str ());
	}
}

#ifdef DEBUG_MODE
using namespace debug;
#define DEBUG_OUT(my_x__) debug::debug_out (my_x__);
#else
#define DEBUG_OUT (x) sizeof(x);
#endif
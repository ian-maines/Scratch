// Copyright Ian Maines
// Debugging utilities.
#pragma once

#define VERBOSE_DEBUGGING

#ifdef _DEBUG
	#include <iostream>
	#define ASSERT(expr) if (!expr) { std::cout << "ASSERT '" << #expr << "' failed.\n"; throw (-1); }
	#ifdef VERBOSE_DEBUGGING
		#include <stdarg.h>
		#include <cstdio>
		inline void DEBUG (const char* fmt, ...)
			{
			char buffer[1000];
			va_list argptr;
			va_start (argptr, fmt);
			vsprintf_s (buffer, 1000, fmt, argptr);
			va_end (argptr);
			std::cout << buffer;
			return;
			}
	#else
		#define DEBUG (fmt, ...) void sizeof(0);
	#endif
#else
	#define DEBUG (fmt, ...) void sizeof(0);
	#define ASSERT(expr,str) void sizeof(0);
#endif
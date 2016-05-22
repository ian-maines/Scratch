// Copyright Ian Maines
// Debugging utilities.
#pragma once

#include "format.h"

#define VERBOSE_DEBUGGING

#ifdef _DEBUG
	#include <iostream>
	#define ASSERT(expr) if (!(expr)) { std::cout << "ASSERT '" << #expr << "' failed.\n"; throw (-1); }
	#ifdef VERBOSE_DEBUGGING
		#include <stdarg.h>
		#include <cstdio>
		template <typename ... Args>
		inline void DEBUG (const std::string& fmt, Args&& ... args)
			{
			std::cout << string_format (std::forward<const std::string> (fmt), std::forward<Args> (args)...);
			return;
			}
	#else
		#define DEBUG (fmt, ...) void sizeof(0);
	#endif
#else
	#define DEBUG (fmt, ...) void sizeof(0);
	#define ASSERT(expr,str) void sizeof(0);
#endif
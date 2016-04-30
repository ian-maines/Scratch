// Copyright Ian Maines
// Debugging utilities.
#pragma once

#ifdef _DEBUG
#include <iostream>
#define ASSERT(expr,str) if (!expr) { std::cout << "ASSERT '" << ##expr << "' failed. " << str << "\n"; throw (-1); }
#else
#define ASSERT(expr,str) void sizeof(0);
#endif
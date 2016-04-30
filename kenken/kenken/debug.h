// Copyright Ian Maines
// Debugging utilities.
#pragma once

#ifdef _DEBUG
#include <iostream>
#define ASSERT(expr) if (!expr) { std::cout << "ASSERT '" << ##expr << "' failed. " << "\n"; throw (-1); }
#else
#define ASSERT(expr,str) void sizeof(0);
#endif
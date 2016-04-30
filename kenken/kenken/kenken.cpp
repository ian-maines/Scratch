// kenken.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "types.h"
#include "debug.h"

int main()
	{
	std::cout << "TEST\n";
	
	std::vector<point> math_locations = {point (location::a0), point (location::a1), point (location::a2)};
	math_group mg (math_expr (6, operation::plus), math_locations);


	std::this_thread::sleep_for (std::chrono::seconds (10));
	return 0;
	}

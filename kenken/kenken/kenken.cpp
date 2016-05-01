// kenken.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "types.h"
#include "location.h"
#include "combination.h"
#include "debug.h"
#include "math_group.h"

int main()
	{
	std::cout << "TEST\n";
	
	//std::vector<point> math_locations = {point (location::a0), point (location::a1), point (location::a2)};
	//math_group mg (math_expr (6, operation::plus), math_locations);

	location l1 (p_to_l (point (location::a0)));
	location l2 (p_to_l (point (location::b3)));

	return 0;
	}

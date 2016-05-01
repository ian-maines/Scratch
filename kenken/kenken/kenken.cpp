// kenken.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "types.h"
#include "location.h"
#include "combination.h"
#include "board.h"
#include "globals.h"
#include "math_group.h"

namespace
{

const std::vector<math_group> board_def = 
							{ math_group ({math_expr (1, operation::minus), locations_t{point (location::a0), point (location::a1)}})
							, math_group ({math_expr (5, operation::minus), locations_t{point (location::b0), point (location::c0)}})
							, math_group ({math_expr (108, operation::mult), locations_t{point (location::d0), point (location::d1), point (location::d2)}})
							, math_group ({math_expr (24, operation::plus), locations_t{point (location::e0), point (location::e1), point (location::e2)}})
							, math_group ({math_expr (10, operation::plus), locations_t{point (location::f0), point (location::f1)}})
							, math_group ({math_expr (3, operation::div), locations_t{point (location::g0), point (location::g1)}})
							, math_group ({math_expr (4, operation::minus), locations_t{point (location::h0), point (location::i0)}})
							, math_group ({math_expr (72, operation::mult), locations_t{point (location::b1), point (location::c1)}})
							, math_group ({math_expr (60, operation::mult), locations_t{point (location::h1), point (location::i1), point (location::h2)}})
							, math_group ({math_expr (17, operation::plus), locations_t{point (location::a2), point (location::b2), point (location::b3)}})
							, math_group ({math_expr (8, operation::minus), locations_t{point (location::c2), point (location::c3)}})
							, math_group ({math_expr (3, operation::minus), locations_t{point (location::f2), point (location::g2)}})
							, math_group ({math_expr (4, operation::div), locations_t{point (location::i2), point (location::i3)}})
							, math_group ({math_expr (8, operation::minus), locations_t{point (location::a3), point (location::a4)}})
							, math_group ({math_expr (3, operation::minus), locations_t{point (location::d3), point (location::d4)}})
							, math_group ({math_expr (15, operation::plus), locations_t{point (location::e3), point (location::e4), point (location::f4)}})
							, math_group ({math_expr (2, operation::minus), locations_t{point (location::f3), point (location::g3)}})
							, math_group ({math_expr (17, operation::plus), locations_t{point (location::h3), point (location::g4), point (location::h4), point (location::i4)}})
							, math_group ({math_expr (24, operation::mult), locations_t{point (location::b4), point (location::c4), point (location::c5)}})
							, math_group ({math_expr (560, operation::mult), locations_t{point (location::a5), point (location::b5), point (location::a6), point (location::a7)}})
							, math_group ({math_expr (11, operation::plus), locations_t{point (location::d5), point (location::d6)}})
							, math_group ({math_expr (2, operation::minus), locations_t{point (location::e5), point (location::e6)}})
							, math_group ({math_expr (4, operation::minus), locations_t{point (location::f5), point (location::f6)}})
							, math_group ({math_expr (18, operation::mult), locations_t{point (location::g5), point (location::g6)}})
							, math_group ({math_expr (21, operation::plus), locations_t{point (location::h5), point (location::i5), point (location::h6)}})
							, math_group ({math_expr (5, operation::minus), locations_t{point (location::b6), point (location::c6)}})
							, math_group ({math_expr (378, operation::mult), locations_t{point (location::i6), point (location::i7), point (location::i8)}})
							, math_group ({math_expr (84, operation::mult), locations_t{point (location::b7), point (location::a8), point (location::b8)}})
							, math_group ({math_expr (1, operation::minus), locations_t{point (location::c7), point (location::c8)}})
							, math_group ({math_expr (5, operation::none), locations_t{point (location::d7)}})
							, math_group ({math_expr (8, operation::minus), locations_t{point (location::e7), point (location::f7)}})
							, math_group ({math_expr (15, operation::plus), locations_t{point (location::g7), point (location::g8)}})
							, math_group ({math_expr (4, operation::minus), locations_t{point (location::h7), point (location::h8)}})
							, math_group ({math_expr (6, operation::plus), locations_t{point (location::d8), point (location::e8), point (location::f8)}})
							};
}

int main()
	{
	std::cout << "TEST\n";
	
	locations_t math_locations = {point (location::a0), point (location::a1), point (location::a2)};
	math_group mg (math_expr (6, operation::plus), math_locations);

	board board (board_def);

	std::cout << "\n" << board.to_string () << "\n";

	location l1 (p_to_l (point (location::a0)));
	location l2 (p_to_l (point (location::b3)));

	return 0;
	}

// Copyright 2019 Ian Maines
#include "pch.h"
#include <iostream>
#include <cstdint>

/* https://projecteuler.net/problem=6
The sum of the squares of the first ten natural numbers is,
1^2 + 2^2 + ... 10^2 = 385

The square of the sum of the first ten natural numbers is,
(1 + 2 + ... 10)^2 = 55^2 = 3025
Hence the difference between the sum of the squares of the first ten natural numbers and the square of the sum is 3028 - 385 = 2640
Find the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum.
*/

namespace
	{

	}

int main()
	{
	// Sum of squares first.
	uint64_t su_o_sq = 0;
	uint64_t sq_o_su = 0;
	for (int i = 1; i <= 100; ++i)
		{
		su_o_sq += i * i;
		sq_o_su += i;
		}
	// Now we need to square the sum.
	sq_o_su = sq_o_su * sq_o_su;

	// Square of sums is going to be larger.
	std::cout << "Difference=" << sq_o_su - su_o_sq;
	}


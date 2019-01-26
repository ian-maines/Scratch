// Copyright 2019 Ian Maines
#include "pch.h"
#include <iostream>
#include <vector>

/* https://projecteuler.net/problem=7
By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.

What is the 10,001st prime number?
*/

namespace
	{
	std::vector<uint32_t> primes;

	bool IsPrime (uint64_t p)
		{
		// Base cases
		if (p == 1 || p == 2) return true;
		if (p % 2 == 0 || p == 0) return false;// Don't accept 0;
		// Can skip even numbers since those would be divisible by 2.
		for (uint32_t i = 3; i < (p / 2); i += 2)
			{
			if (p % i == 0) return false;
			}
		return true;
		}
	}

int main()
	{
	// We actually have code for this in ID03 Largest Prime Factor
	primes.push_back(2);
	for (uint32_t i = 3; primes.size() < 10001; ++i)
		{
		if (IsPrime (i))
			{
			primes.push_back (i);
			std::cout << i << ",";
			}
		}
	std::cout << std::endl << "Size of primes vector = " << primes.size () << std::endl;
	std::cout << primes.back();

	return 0;
	}

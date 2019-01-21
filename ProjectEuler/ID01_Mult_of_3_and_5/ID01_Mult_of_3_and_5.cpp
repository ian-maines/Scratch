// Copyright 2019 Ian Maines
#include "pch.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

/* Problem statement from https://projecteuler.net/problem=1
If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.
*/

// Test data
namespace data
	{
	std::vector<uint32_t> vec;
	}

int main()
	{
	// Initialize data
	for (int i = 1; i < 1000; ++i)
		{
		data::vec.push_back(i);
		}
	// Display for debugging
    std::cout << "Input Dataset:\n"; 
	int i = 0;
	std::for_each (data::vec.begin (), data::vec.end (), [](uint32_t i) {std::cout << i << "," << (((i%20)==0)?"\n":""); ++i;});
	std::cout << std::endl << std::endl;

	uint64_t sum = 0;
	// Find all of the multiples of 3 or 5 and add them up.
	std::for_each (data::vec.begin (), data::vec.end (), [&sum](uint32_t i) {if (((i % 3) == 0) || ((i % 5) == 0)) {std::cout << i << ","; sum += i;}});
	std::cout << "\n\n Answer: " << sum << std::endl;
	}


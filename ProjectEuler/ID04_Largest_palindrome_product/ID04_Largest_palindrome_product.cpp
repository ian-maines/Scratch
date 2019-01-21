// Copyright 2019 Ian Maines
#include "pch.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

/* https://projecteuler.net/problem=4
A palindromic number reads the same both ways.The largest palindrome made from the product of two 2 - digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3 - digit numbers.
*/

namespace
	{
	std::vector<uint16_t> three_digit_numbers;

	bool is_palindrome (uint32_t p)
		{
		// Algorithm: We could do this with or without converting to a string.
		// Without:
		// 1. Determine how many digits are in the number.
		// 2. Loop over all relevant digits comparing to their complimentary digit on the other side of the number.
		// With:
		//	1. Convert to string.
		//	2. Loop over string comparing complimentary digits
		
		// Going to do it by converting to a string.
		// Largest a 32-bit number can be is 4294967295, so 10 digits + one for the null
		char str[11];
		_itoa_s (p, str, _countof(str), 10);// The 10 means display in decimal

		// Now we need to compare
		const size_t len = strlen(str);
		for (uint16_t i = 0; i < len / 2; ++i)
			{
			if (str[i] != str[len - i - 1])
				{
				return false;
				}
			}
		std::cout << "Palindrome = " << str << std::endl;
		return true;
		}
	}

int main ()
	{
	// Algorithm - O(n^2) loop over all 3-digit numbers. (Unclear from problem statement if that starts at 1 or 100 - hmm. Assume for now means at 100)
	// Track palindromes and update if we have the largest

	// First initialize our 3-digit numbers
	for (uint16_t i = 100; i < 1000; ++i)
		{
		three_digit_numbers.push_back(i);
		}


	// Now loop over them doing multiplication
	// Need to keep track of our position - no need to duplicate effort by using a number we've already hit.
	uint16_t pos = 0;
	uint32_t largest = 0;
	std::for_each (three_digit_numbers.begin (), three_digit_numbers.end (), [&pos, &largest](uint16_t num) 
		{
		std::for_each (three_digit_numbers.begin () + pos, three_digit_numbers.end (), [&num, &largest](uint16_t second) 
			{
			uint32_t prod = num*second;
			if (is_palindrome (prod))
				{
				if (prod > largest)
					{
					largest = prod;
					std::cout << "Updated largest to " << largest << std::endl;
					}
				}
			});
		});

	std::cout << "Largest Palindrome = " << largest << std::endl;
	}

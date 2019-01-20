// ID05_Smallest_multiple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cstdint>

/*https://projecteuler.net/problem=5
2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?
*/

namespace
	{
	// Algorithm: Brute force would be to just start searching. We know the number must be even so that it can be divided by two,
	// and must also be greater than or equal to the largest value in the input range.
	// 
	uint32_t find_smallest_number (const uint32_t cap)
		{
		uint64_t i = cap;
		while (true)// Largest number of times we could loop would be cap!, which is a lot.
			{
			bool bNoGood = false;
			for (uint32_t j = 1; j <= cap; ++j)
				{
				if (i % j != 0)
					{
					// If we're not evenly divisible, no good.
					bNoGood = true;
					break;
					}
				}
			// If we get out of the loop and bNoGood is still false, we found it
			if (!bNoGood)
				{
				// Quit and go home.
				return i;
				}
			++i;
			}
		}

	}

int main()
	{


	}

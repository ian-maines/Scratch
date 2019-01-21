/// Copyright 2019 Ian Maines
#include "pch.h"
#include <iostream>
#include <cstdint>
#include <vector>

/* https://projecteuler.net/problem=3
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143 ?
*/

namespace
	{
	const uint64_t input = 600'851'475'143;

	// For now assume we have to build the prime number list ourselves as part of the challenge.
	std::vector<uint32_t> primes = { 2 };

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

	void AddNextPrime ()
		{
		// Base case - 2 is the only even prime number.
		if (primes.back () == 2)
			{
			primes.push_back (3);
			return;
			}
		// Build prime number table. We need to get up to input/2 to find all prime factors. (Technically we could do sqrt(input))
		// FIXME: For now this is O(n^2), can we make it better? It's stupid slow
		for (uint32_t i = primes.back()+2; i < input / 2; ++i)
			{
			if (IsPrime (i))
				{
				primes.push_back (i);
				std::cout << i << ",";
				break;
				}
			}
		return;
		}
	}

int main()
	{
	// Need to find prime factors of the monster number indicated above.
	// Algorithm:
	// 1. Create or obtain prime number table. <- Alternately we could generate the prime number table on an as-needed bases with the following algorithm:
	//	1.1 Begin doing prime factorization with only "2" in the table. As soon as "2" while (current number isn't prime && current prime # isn't a factor) {Find new prime #}
	//	**SELECTED THIS METHOD**
	// 2. Begin attempting to divide the number by each ascending prime number.
	//		When one of them works, insert that number into the prime number table.
	// 3. Loop back to step two. **FIXME** Do we need to start back at 2 again, or can we maintain our position in the prime number table?
	//		99.999999876% sure we can maintain our position in the table, but if we run into issues, that would be a good palce to look.

	// This is mostly for debugging - it's not necessary for the solution.
	std::vector<uint64_t> factors;

	// Now we do factorization
	uint64_t working = input;
	uint64_t current_prime = primes.back ();
	// Once working is prime, we're done.
	while (!IsPrime (working))
		{
		// If working is evently divisible by current prime, it's a factor. Factor it out.
		auto mod = (working % current_prime);
		if (mod == 0)
			{
			working /= current_prime;
			factors.push_back(current_prime);
			}
		else
			{
			AddNextPrime ();
			current_prime = primes.back();
			}
		}
	std::cout<< "Largest Factor: " << working << std::endl; 
	}
//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     main.cpp for scratch challenges.
// Date:            04/25/2015
//
//************************************************************

#include <iostream>
#include <limits>

#include "time.h"

using namespace std;

namespace
{
// Recursive.
const unsigned int mask (0x01);
bool power_of_2 (unsigned int x, unsigned int call_depth = 0)
	{
	// 
	int count = 0;
	int localmask = 1;
	while (localmask)
		{
		if (localmask & x)
			{
			count++;
			}
		localmask = localmask << 1;
		}
	return (count == 1);
	// Method 2 below.
	// trivial case.
	if (x == 0)
		{
		return false;
		}

	if (x & mask)
		{
		if (!(x ^ mask))
			{
			return true;
			}
		else
			{
			return false;
			}
		}
	else
		{
		return power_of_2 (x >> 1, call_depth + 1);
		}
	}

}

int main ()
	{
	for (unsigned int n = 0; n < std::numeric_limits<unsigned int>::max (); n++)
		{
		if (power_of_2 (n))
			{
			cout << n << " is a power of 2." << endl;
			}
		}

	// Exit message
	cout << endl << "Press ENTER to exit...";
	std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
	return 0;
	}
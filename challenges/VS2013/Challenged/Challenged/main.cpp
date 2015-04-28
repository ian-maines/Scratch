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
// This is order O(sizeof(t))
bool power_of_2 (unsigned long long int x)
	{
	int count = 0;
	for (unsigned long long int localmask = 1; localmask; localmask = localmask << 1)
		{
		if (localmask & x)
			{
			count++;
			}
		}
	return (count == 1);
	}

}

int main ()
	{
	cout << "sizeof (unsigned long int): " << sizeof (unsigned long long int) << endl;
	for (unsigned long long int n = 1; n; n = n << 1)
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
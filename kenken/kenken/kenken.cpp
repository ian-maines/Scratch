// kenken.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "types.h"

int main()
	{
	std::cout << "TEST\n";
	
	point pt (location::b0);

	board_t board;

	board.set (point (location::a1), 1);
	board.set (point (location::b0), 5);


	std::cout << "Point: " << static_cast<int>(pt.m_pt.first) << "," << static_cast<int>(pt.m_pt.second) << "\n";
	std::cout << "Board:\n" << board.to_string ();

	std::this_thread::sleep_for (std::chrono::seconds (10));
	return 0;
	}

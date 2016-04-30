// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>
#include <numeric>
#include <string>
#include <algorithm>

#include "globals.h"
#include "element.h"
#include "location.h"

struct board_t
	{
	board_t ()
		{
		clear ();
		}

	void clear () { memset (&m_board, 0, sizeof (m_board)); }


	void set (point point, element val)
		{
		m_board[point.m_pt.first][point.m_pt.second] = val;
		}

	std::string to_string ()
		{
		static_assert (board_size < std::numeric_limits<unsigned char>::max (), "Board size should not exceed 9, 255 will break this function.");
		std::string val;
		for (unsigned char y = 0; y < board_size; ++y)
			{
			for (unsigned char x = 0; x < board_size; ++x)
				{
				val += "[" + std::to_string ((m_board[x][y])) + "]";
				if (board_size - 1 == x)
					{
					val += "\n";
					}
				}
			}
		return val;
		}

	element m_board[board_size][board_size];
	};
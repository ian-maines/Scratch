// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>
#include <numeric>
#include <string>

using element = unsigned char;
using pt = std::pair<unsigned char, unsigned char>;
constexpr unsigned int board_size = 9;

// Defines location on the board. 9x9 is 0-8, a-i.
enum class location : unsigned char
	{
	a0 = 0,	a1,	a2,	a3,	a4,	a5,	a6,	a7,	a8,
	b0,	b1,	b2,	b3,	b4,	b5,	b6,	b7,	b8,	
	c0, c1,	c2,	c3,	c4,	c5,	c6,	c7,	c8,	
	d0,	d1,	d2,	d3,	d4,	d5,	d6,	d7,	d8,
	e0,	e1,	e2,	e3,	e4,	e5,	e6,	e7,	e8,
	f0,	f1,	f2,	f3,	f4,	f5,	f6,	f7,	f8,
	g0,	g1,	g2,	g3,	g4,	g5,	g6,	g7,	g8,
	h0,	h1,	h2,	h3,	h4, h5,	h6,	h7,	h8,
	i0,	i1,	i2,	i3,	i4,	i5,	i6,	i7,	i8,
	};


pt l_to_pt (location l)
	{
	const auto val = static_cast<unsigned char>(l);
	// l / 9 is the row, l % 9 is the column
	return std::make_pair ((val / 9), (val % 9));
	}

struct point
	{
	explicit point (location l)
		: m_pt (l_to_pt (l))
		{}

	unsigned char x () { return m_pt.first; }
	unsigned char y () { return m_pt.second; }

	pt m_pt;
	};

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
				val += "[" + std::to_string((m_board[x][y])) + "]";
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


// Vector of elements representing a complete set of one combination valid for a math group.
struct combination
	{
	combination (std::vector<element>&& combination)
		: m_combination  (combination)
		, m_valid (true)
		{}
	// vector of chars representing this combination.
	std::vector<element> m_combination;
	bool m_valid;	// Set to false if this combination becomes impossible in the parent math group.
	};

class math_group
	{

	private:
		// All possible combinations for this math group.
		std::vector<combination> m_combinations;
	};


class cell
	{
	public:
		cell ()
			{}

	private:
		// Vector of possible combinations (from math group)
		std::vector<combination> m_combinations;
		// Vector of numbers this cell may NOT be.
		std::vector<element> m_not;
	};

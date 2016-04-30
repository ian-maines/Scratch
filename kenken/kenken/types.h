// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>
#include <numeric>
#include <string>
#include <algorithm>

#include "debug.h"

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

pt l_to_pt (location l);

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
		{ clear ();	}

	void clear () { memset (&m_board, 0, sizeof (m_board)); }
	void set (point point, element val);
	std::string to_string ();

	element m_board[board_size][board_size];
	};


// Vector of elements representing a complete set of one combination valid for a math group.
class combination
	{
	public:
		combination (size_t size)
			: m_combination (size, 1)	// Init values to 1.
			, m_valid (true)
			{}

		combination (const std::vector<element>& combination)
			: m_combination  (combination)
			, m_valid (true)
			{}

		combination (const std::vector<element>&& combination)
			: m_combination (std::move(combination))
			, m_valid (true)
			{}

		bool is_valid () { return m_valid; }
		void invalidate () { m_valid = false; }

		inline bool operator== (const combination& rhs) const
			{
			return (m_combination == rhs.m_combination);
			}

		inline bool operator!= (const combination& rhs) const
			{
			return !(*this == rhs);
			}

		inline auto begin () { return m_combination.begin (); }
		inline auto begin () const { return m_combination.cbegin (); }
		inline auto end () { return m_combination.end (); }
		inline auto end () const { return m_combination.cend (); }

		inline operator std::vector<element> () const { return m_combination; }

	private:
		// vector of chars representing this combination.
		std::vector<element> m_combination;
		bool m_valid;	// Set to false if this combination becomes impossible in the parent math group.
	};

enum class operation
	{
	none = 0,	// A single element of the grid pre-identified.
	plus,
	minus,
	mult,
	div
	};

class math_expr
	{
	public:
		math_expr (unsigned int val, operation op)
			: m_val (val)
			, m_op (op)
			{}

		unsigned int val () const { return m_val; }
		operation op () const { return m_op; }

	private:
		// What this expression evaluates to.
		unsigned int m_val;
		// Math operation used to obtain val.
		operation m_op;
	};

class math_group
	{
	public:
		math_group (math_expr expr, std::vector<point> locations)
			: m_expr (expr)
			, m_locations (locations)
			, m_num_locations (locations.size ())
			{
			m_combinations = std::move(_build_combinations ());
			}


	private:
		// Determines if any of the combinations are invalid based on only the information within this math group.
		bool local_validate ()
			{

			}

		std::vector<combination> _build_combinations () const;
		// Expression for evaluating this math group.
		math_expr m_expr;
		// All locations in this math group.
		std::vector<point> m_locations;
		// All possible combinations for this math group.
		std::vector<combination> m_combinations;
		// Number of elements for quick access.
		const unsigned char m_num_locations;
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

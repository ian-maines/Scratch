// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>
#include <algorithm>

#include "combination.h"

// Returns false if the given combination is not valid with respect to itself (i.e. overlapping values in a row/column)
bool combination_locally_valid (const combination& combo);

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
		math_group (math_expr expr, locations_t locations)
			: m_expr (expr)
			, m_locations (locations)
			, m_num_locations (locations.size ())
			{
			m_combinations = std::move (_build_combinations ());
			}

	private:

		combinations_t _build_combinations () const;
		// Expression for evaluating this math group.
		math_expr m_expr;
		// All locations in this math group.
		locations_t m_locations;
		// All possible combinations for this math group.
		// Each combination directly correlates to its corresponding index in m_locations
		// I.e. if m_locations is [a0, a1, a2] and one combination is [2,3,6] then grid[a0] == 2, grid[a1] == 3, etc.
		combinations_t m_combinations;
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


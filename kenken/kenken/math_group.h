// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>

#include "combination.h"

// First element is the row/column, second is array of values taken.
using rep_t = std::map<unsigned char, std::vector<element>>;

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

inline std::string operation_to_string (operation op)
	{
	switch (op)
		{
		case operation::none:
			return std::string ();
		case operation::plus:
			return std::string ("+");
		case operation::minus:
			return std::string ("-");
		case operation::mult:
			return std::string ("*");
		case operation::div:
			return std::string ("/");
		default:
			ASSERT (!"Unknown operation!");
			return std::string ();
		}
	}

class math_expr
	{
	public:
		math_expr (unsigned int val, operation op)
			: m_val (val)
			, m_op (op)
			{}

		unsigned int val () const { return m_val; }
		operation op () const { return m_op; }

		std::string to_string () const
			{
			return std::to_string (m_val) + operation_to_string (m_op);
			}

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

		const locations_t& get_locations () const { return m_locations; }
		const combinations_t& get_combinations () const { return m_combinations;}
		const math_expr& get_expr () const { return m_expr; }

		std::string to_string () const;

		const rep_t& get_row_rep () const { return m_row_rep; }
		const rep_t& get_col_rep () const { return m_col_rep; }

		// Update row/column representation.
		void refresh_rep ();

	private:

		combinations_t _build_combinations ();
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
		// Values this math group definitely represents for a row/column.
		rep_t m_row_rep;
		rep_t m_col_rep;
	};

using math_group_ptr = std::shared_ptr<math_group>;
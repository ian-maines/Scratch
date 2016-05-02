// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <memory>

#include "globals.h"
#include "element.h"
#include "location.h"
#include "math_group.h"

enum element_vals { undetermined = 0 };

class board_element
	{
	public:
		board_element (math_group_ptr parent_math_group)
			: m_value (undetermined)	//  Value of 0 means undetermined.
			, m_math_group (parent_math_group)
			{}

		board_element (board_element&& rhs)
			: m_value (rhs.m_value)
			, m_math_group (rhs.m_math_group)
			{}

		element GetValue () const { return m_value; }
		void SetValue (element val) { m_value = val; }

	private:
		element m_value;
		math_group_ptr m_math_group;
		// TODO: Column and row occupancies.
	};

using board_element_ptr = std::unique_ptr<board_element>;

class board
	{
	public:
		board (const std::vector<math_group_ptr>& math_groups);

		void set_value (point point, element val);
		std::string to_string ();

		board_element_ptr& get (point pt);
		void set (point pt, board_element_ptr&& val);

	private:
		void _build (const std::vector<math_group_ptr>& mgs);
		int _point_to_index (point pt) const;

		std::vector<board_element_ptr> m_board;
	};
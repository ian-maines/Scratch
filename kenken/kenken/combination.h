// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include "types.h"
#include "location.h"
#include "element.h"

using combination_elements = std::map<point, element>;

// Vector of elements representing a complete set of one combination valid for a math group.
class combination
	{
	public:
		combination (const combination_elements& combination)
			: m_combination (combination)
			, m_valid (true)
			{}

		combination (const combination_elements&& combination)
			: m_combination (combination)
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

		std::string to_string () const
			{
			std::string str ("[");
			std::for_each (m_combination.begin (), m_combination.end (), [&str](auto combo)
				{ str.push_back ('0' + (int)combo.second); });
			str.push_back (']');
			return str;
			}

		inline auto begin () { return m_combination.begin (); }
		inline auto begin () const { return m_combination.cbegin (); }
		inline auto end () { return m_combination.end (); }
		inline auto end () const { return m_combination.cend (); }
		inline auto size () const { return m_combination.size (); }

	private:
		// vector of chars representing this combination.
		combination_elements m_combination;
		// Set to false if this combination becomes impossible in the parent math group.
		bool m_valid;
	};

using combinations_t = std::vector<combination>;
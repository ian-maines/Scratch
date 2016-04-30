// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>

#include "types.h"
#include "location.h"
#include "element.h"

// Vector of elements representing a complete set of one combination valid for a math group.
class combination
	{
	public:
		combination (size_t size, const locations_t& locations)
			: m_combination (size, 1)	// Init values to 1.
			, m_valid (true)
			, m_locations (locations)
			{}

		combination (const elements& combination, const locations_t& locations)
			: m_combination (combination)
			, m_valid (true)
			, m_locations (locations)
			{}

		combination (const elements&& combination, const locations_t& locations)
			: m_combination (std::move (combination))
			, m_valid (true)
			, m_locations (locations)
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
		elements m_combination;
		// Set to false if this combination becomes impossible in the parent math group.
		bool m_valid;
		// Reference to the location set of the parent math group.
		const locations_t& m_locations;
	};

using combinations_t = std::vector<combination>;
// Copyright 2016 Ian Maines.
// 12/18/2016 12:57PM
// Counting Castles Solution - projecteuler.net/problem=502

#include "stdafx.h"
#include <vector>

namespace
{
// Can either be a block or free space.
struct element_t
	{
	size_t m_w;	// Size of block/free space.
	using row = std::vector<element_t>;
	using rows = std::vector<row>;
	rows m_children;	// If block, vector of possible children.
	bool m_isBlock;	// false if empty space

	element_t (size_t w, bool isBlock)
		: m_w (w)
		, m_isBlock (isBlock)
		{}
	};

// returns a vector of combinations that fill the specified space.
// Recurseive. Final iteration pushes back.
element_t::rows fill_space (element_t::row current, size_t remaining_space, element_t::rows& built_array)
	{
	_ASSERT (current.size () >= 1);
	// Recursive end
	if (remaining_space == 0)
		{
		built_array.push_back(current);	// Nada - maybe we want to consider this an error?
		return;
		}

	element_t::rows my_out;
	for (size_t s = remaining_space; s > 0; --s)
		{
		// Figure out what to insert.
		if ((current.end () - 1)->m_isBlock)
			{	// Empty space
			current.push_back (element_t (s, false));
			}
		else
			{	// Block
			current.push_back (element_t (s, true));
			}
		fill_space (current, remaining_space - s, built_array);
		}
	}

// Iterate over all of the posibilities, recurse on them.
void build_possible_children (element_t& parent)
	{
	element_t::rows r;

	// Iterate over all of the possible starting combinations of either block or empty space.
	for (size_t s = parent.m_w; s > 0; --s)
		{
		// For each of these, we can either have empty space or block.
		element_t::row es_r;	// Empty space start.
		es_r.push_back (element_t (s, false));
		fill_space (es_r, parent.m_w - s, r);

		element_t::row b_r;	// Block start.
		b_r.push_back (element_t (s, true));
		fill_space (b_r, parent.m_w - s, r);
		}

	std::swap (parent.m_children, r);

	// Now iterate over everything we just created and recurse there.
	for (auto& child : parent.m_children)
		{
		for (auto& elem : child)
			{
			if (elem.m_isBlock)
				{
				build_possible_children (elem);
				}
			}
		}
	}
}


int _tmain (int argc, _TCHAR* argv[])
	{
	// This is in essence a tree. STL doesn't provide a tree container, but we can simulate one with vectors of vectors.

	const int grid_w = 4;
	const int grid_h = 2;

	// Starting element is always width w.
	element_t start (grid_w, true);

	build_possible_children (start);




	return 0;
	}


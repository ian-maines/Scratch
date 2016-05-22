#include <type_traits>

#include "globals.h"
#include "math_group.h"

namespace
{
struct val_count
	{
	val_count (unsigned char val, unsigned int ac)
		: value (val)
		, appearance_count (ac)
		{}

	unsigned char value;
	unsigned int appearance_count;
	};

// Helper to build row/column representation.
struct rep_builder
	{
	// Builds the skeleton for a row or column's value appearance state.
	rep_builder ()
		{
		// iterate over all rows/columns
		for (unsigned char i (0); i < board_size; ++i)
			{
			std::vector<val_count> vec;
			// Iterate over all values.
			for (unsigned char j (1); j <= board_size; ++j)
				{
				vec.push_back (val_count (j, 0));
				}
			rep.insert (std::make_pair (i, vec));
			}
		}
	// <row/col, <value, appearance count>
	std::map<unsigned char, std::vector<val_count>> rep;
	};

// Custom class basically to represent an int - customized incrementing and rollover.
class num_set
	{
	public:
		num_set (size_t size)
			: m_elems (size, 1)
			{
			}
		// prefix (++thing)
		num_set& operator++ ()
			{
			_inc ();
			return *this;
			}

		// postfix (thing++)
		num_set operator++ (int)
			{
			// Implemented in terms of prefix operator.
			num_set result (*this);
			++(*this);
			return result;
			}

		// Returns true if this num_set represents the max value allowed for the size of the current math operation and board
		// (i.e. '99' for a 1x2 slot on a 9x9 board, '66' for the same slot on a 6x6 board).
		bool is_max () const
			{
			bool bMax (true);
			std::for_each (m_elems.begin (), m_elems.end (), [&bMax](auto i) { bMax &= (i == board_size); });
			return bMax;
			}

		// Returns the value this num_set currently represents based on the opval i.e. 6 for a [2,3] mult, 7 for a [1,2,4] plus.
		// Return value of zero for division operations where the numbers aren't divisible.
		unsigned int opval (const operation op) const
			{
			switch (op)
				{
				case operation::plus:
					{
					int sum (0);
					std::for_each (m_elems.begin (), m_elems.end (), [&sum](auto i) { sum += i; });
					return sum;
					break;
					}
				case operation::minus:
					{
					ASSERT (2 == m_elems.size ());
					return std::abs (m_elems[0] - m_elems[1]);
					break;
					}
				case operation::mult:
					{
					int prod (1);
					std::for_each (m_elems.begin (), m_elems.end (), [&prod](auto i) { prod *= i; });
					return prod;
					break;
					}
				case operation::div:
					{
					ASSERT (2 == m_elems.size ());
					unsigned char e0 (m_elems[0]);
					unsigned char e1 (m_elems[1]);

					if (e0 > e1)
						{
						// If they're not evenly divisible, return 0.
						if (e0 % e1)
							{
							return 0;
							}
						else
							{
							return (e0 / e1);
							}
						}
					else
						{
						// If they're not evenly divisible, return 0.
						if (e1 % e0)
							{
							return 0;
							}
						else
							{
							return (e1 /e0);
							}
						}
					break;
					}
				case operation::none:
				default:
					{
					ASSERT (!"Case shouldn't be handled here.");
					return 0;
					break;
					}
				}
			}

		const elements& get () const
			{
			return m_elems;
			}

	private:
		// Recursive. Allows overflow.
		bool _inc () { return _inc (m_elems.end () - 1); }
		// 
		bool _inc (std::vector<unsigned char>::iterator pos)
			{
			if (*pos == board_size)
				{
				*pos = 1;
				return _inc (pos - 1);
				}
			else
				{
				++(*pos);
				return false;
				}
			}

		elements m_elems;
	};
}	// namespace {anonymous}

std::string math_group::to_string () const
	{
	return string_format ("%s (%u locations): %u combinations.", m_expr.to_string ().c_str (), m_locations.size (), m_combinations.size ());
	}

// Refreshes the numbers represented in each row/column.
// FIXME: Rows and cols are swapped.
void math_group::refresh_rep ()
	{
	// First discover a count for each of the value for all rows/columns represented by this math group.
	// FIXME: m_combinations may contain combinations that have been invalidated. Check for that!!!
	rep_builder rows, cols;
	for (auto& combo : m_combinations)
		{
		std::for_each (combo.begin (), combo.end (), [&rows, &cols, this](auto combo_elem)
			{
			// Grab the value for this combo element.
			unsigned char value = combo_elem.second;

			// Go over the rows first.
			// Grab the row for this combo element.
			auto& row = rows.rep[combo_elem.first.row ()];
			// Find the appearance count for the value we've got
			auto row_val_ac (std::find_if (row.begin (), row.end (), [&value](auto location) 
				{
				return location.value == value;
				}));
			// Bump the appearance count.
			ASSERT (row_val_ac != row.end ());
			++(row_val_ac->appearance_count);

			// Now columns
			// Grab the column for this combo element.
			auto& col = cols.rep[combo_elem.first.col ()];
			// Find the appearance count for the value we've got
			auto col_val_ac (std::find_if (col.begin (), col.end (), [&value](auto location)
				{
				return location.value == value;
				}));
			// Bump the appearance count.
			ASSERT (col_val_ac != col.end ());
			++(col_val_ac->appearance_count);
			});
		}

	// Now update member variable representation.

	}

void math_group::_build_combinations ()
	{
	// Allows two iterators in for loop
	struct duiter_t
		{
		locations_t::const_iterator lociter;
		elements::const_iterator comboiter;
		duiter_t (locations_t::const_iterator li, elements::const_iterator ci): lociter (li), comboiter (ci){}
		};

	DEBUG ("Building combinations for %s (size: %d)\n", m_expr.to_string ().c_str (), m_locations.size ());

	if (m_expr.op () == operation::none)	// Special case
		{
		ASSERT (m_locations.size () == 1);
		combination_elements elems;	// Map of location-to-value
		elems.insert (std::make_pair (m_locations[0], m_expr.val ()));
		m_combinations.emplace_back (combination (elems));
		}
	else
		{
		num_set set (m_locations.size ());
		while (!set.is_max ())
			{
			// If this combination satisfies the math expression
			if (set.opval (m_expr.op ()) == m_expr.val ())
				{
				// Allow duplicates (value sets, not positions).
				combination_elements elems;	// Elements of the current combination
				// Insert the location-value pairs into the element.
				for (duiter_t i (m_locations.cbegin (), set.get ().cbegin ()); i.lociter != m_locations.end (); ++i.lociter, ++i.comboiter)
					{
					elems.insert (std::make_pair (*i.lociter, *i.comboiter));
					}
				// Build the combination
				combination c (elems);
				if (combination_locally_valid (c))	// Don't add the combination if it's invalid (i.e. has two '1's in the same row.
					{
					DEBUG ("Adding combination %s\n", c.to_string ().c_str ());
					m_combinations.emplace_back (std::move (c));
					}
				else
					{
					DEBUG ("Rejecting combination %s due to local invalid state\n", c.to_string ().c_str ());
					}
				}
			++set;
			}
		}
	refresh_rep ();
	return;
	}

bool combination_locally_valid (const combination& combo)
	{
	for (auto& i = combo.begin (); i != combo.end (); ++i)
		{
		auto dupe = std::find_if (combo.begin (), combo.end (), [&i](auto elem) { return elem.second == i->second; });
		// Ignore the current element
		if (dupe != i)
			{
			// Value is the same, invalidate if both row and column are not different.
			if ((i->first.row () == dupe->first.row ()) || (i->first.col () == dupe->first.col ()))
				{
				return false;
				}
			}
		}
	return true;
	}

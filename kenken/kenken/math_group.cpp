#include <type_traits>

#include "globals.h"
#include "math_group.h"

namespace
{
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

		bool is_max () const
			{
			bool bMax (true);
			std::for_each (m_elems.begin (), m_elems.end (), [&bMax](auto i) { bMax &= (i == board_size); });
			return bMax;
			}

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
}

combinations_t math_group::_build_combinations () const
	{
	// Allows two iterators in for loop
	struct duiter_t
		{
		locations_t::const_iterator lociter;
		elements::const_iterator comboiter;
		duiter_t (locations_t::const_iterator li, elements::const_iterator ci): lociter (li), comboiter (ci){}
		};
	combinations_t combinations;

	if (m_expr.op () == operation::none)	// Special case
		{
		ASSERT (m_locations.size () == 1);
		combination_elements elems;	// Map of location-to-value
		elems.insert (std::make_pair (m_locations[0], m_expr.val ()));
		combinations.emplace_back (combination (elems));
		}
	else
		{
		num_set set (m_locations.size ());
		while (!set.is_max ())
			{
			if (set.opval (m_expr.op ()) == m_expr.val ())
				{
				// Allow duplicates (value sets, not positions).
				combination_elements elems;
				for (duiter_t i (m_locations.cbegin (), set.get ().cbegin ()); i.lociter != m_locations.end (); ++i.lociter, ++i.comboiter)
					{
					elems.insert (std::make_pair (*i.lociter, *i.comboiter));
					}
				combination c (elems);
				if (combination_locally_valid (c))
					{
					DEBUG ("Adding combination %s\n", c.to_string ().c_str ());
					combinations.emplace_back (std::move (c));
					}
				else
					{
					DEBUG ("Rejecting combination %s due to local invalid state\n", c.to_string ().c_str ());
					}
				}
			++set;
			}
		}
	return combinations;
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

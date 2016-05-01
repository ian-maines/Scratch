#include <type_traits>

#include "globals.h"
#include "math_group.h"

namespace
{

class num_set
	{
	public:
		num_set (size_t size)
			: m_elems (size)
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

		unsigned int sum () const
			{
			int sum (0);
			std::for_each (m_elems.begin (), m_elems.end (), [&sum](auto i) { sum += i; });
			return sum;
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
	switch (m_expr.op ())
		{
		case operation::none:
			{
			ASSERT (m_locations.size () == 1);
			combination_elements elems;	// Map of location-to-value
			elems.insert (std::make_pair (m_locations[0], m_expr.val ()));
			combinations.emplace_back (combination (elems));
			break;
			}
		case operation::plus:
			{
			num_set set (m_locations.size ());
			while (!set.is_max ())
				{
				if (set.sum () == m_expr.val ())
					{
					// Allow duplicates
					combination_elements elems;
					for (duiter_t i (m_locations.cbegin (), set.get ().cbegin ()); i.lociter != m_locations.end (); ++i.lociter, ++i.comboiter)
						{
						elems.insert (std::make_pair(*i.lociter, *i.comboiter));
						}
					combinations.push_back (elems);
					}
				++set;
				}
			return combinations;
			break;
			}
		case operation::minus:
			{

			break;
			}
		case operation::mult:
			{

			break;
			}
		case operation::div:
			{

			break;
			}
		}
	return combinations;
	}

bool math_group::local_validate ()
	{
	for (auto& combo : m_combinations)
		{
		ASSERT (combo.size () == m_locations.size ());
		for (auto& i = combo.begin (); i != combo.end (); ++i)
			{
			auto dupe = std::find_if (combo.begin (), combo.end (), [&i](auto elem) { return elem.second == i->second; });
			// Ignore the current element
			if (dupe != i)
				{
				// Value is the same, invalidate if both row and column are not different.
				if ((i->first.row () == dupe->first.row ()) && (i->first.col () == dupe->first.col ()))
					{
					combo.invalidate ();
					}
				}
			}
		}
	}

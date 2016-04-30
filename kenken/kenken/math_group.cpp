#include "globals.h"
#include "math_group.h"

namespace
{
class num_set
	{
	public:
		num_set (size_t size)
			: m_v (size)
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
			std::for_each (m_v.begin (), m_v.end (), [&bMax](auto i) { bMax &= (i == board_size); });
			return bMax;
			}

		unsigned int sum () const
			{
			int sum (0);
			std::for_each (m_v.begin (), m_v.end (), [&sum](auto i) { sum += i; });
			return sum;
			}

		auto get () const
			{
			return m_v;
			}

	private:
		// Recursive. Allows overflow.
		bool _inc () { return _inc (m_v.end () - 1); }
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

		std::vector<unsigned char> m_v;
	};
}

combinations_t math_group::_build_combinations () const
	{
	combinations_t combinations;
	switch (m_expr.op ())
		{
		case operation::none:
			{
			elements elems;
			elems.push_back (m_expr.val ());
			combination c (std::move (elems), m_locations);
			combinations.push_back (c);
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
					combination c (set.get (), m_locations);
					combinations.push_back (c);
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
		// Make sure no column dupes
		for (int i = 0; i < combo.size (); ++i)
			{

			}
		}
	return false;	// TODO
	}

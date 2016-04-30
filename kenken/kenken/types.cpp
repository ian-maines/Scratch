// Kenken supporting types
// Copyright Ian Maines 4/30/16

#include "types.h"


namespace
{
class sum_set
	{
	public:
		sum_set (size_t size)
			: m_v (size, 1)
			{
			}
		// prefix (++thing)
		sum_set& operator++ ()
			{
			_inc ();
			return *this;
			}

		// postfix (thing++)
		sum_set operator++ (int)
			{
			// Implemented in terms of prefix operator.
			sum_set result (*this);
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
			std::for_each (m_v.begin (), m_v.end (), [&sum](auto i) { sum+= i; });
			return sum;
			}

		auto get () const
			{
			return m_v;
			}

	private:
		// Recursive. Returns true on overflow.
		bool _inc () { ASSERT (!is_max (), "OVERFLOW");  return _inc (m_v.end () - 1); }

		bool _inc (std::vector<unsigned char>::iterator pos)
			{
			if (*pos == board_size)
				{
				if (pos == m_v.begin ()) // Special processing for the most significant digit.
					{
					return false;
					}
				else
					{
					*pos = 1;
					return _inc (pos - 1);
					}

				}
			else
				{
				++(*pos);
				}
			}

		std::vector<unsigned char> m_v;
	};
}

pt l_to_pt (location l)
	{
	const auto val = static_cast<unsigned char>(l);
	// l / 9 is the row, l % 9 is the column
	return std::make_pair ((val / 9), (val % 9));
	}

void board_t::set (point point, element val)
	{
	m_board[point.m_pt.first][point.m_pt.second] = val;
	}

std::string board_t::to_string ()
	{
	static_assert (board_size < std::numeric_limits<unsigned char>::max (), "Board size should not exceed 9, 255 will break this function.");
	std::string val;
	for (unsigned char y = 0; y < board_size; ++y)
		{
		for (unsigned char x = 0; x < board_size; ++x)
			{
			val += "[" + std::to_string ((m_board[x][y])) + "]";
			if (board_size - 1 == x)
				{
				val += "\n";
				}
			}
		}
	return val;
	}

std::vector<combination> math_group::_build_combinations () const
	{
	std::vector<combination> vals;
	switch (m_expr.op ())
		{
		case operation::none:
			{
			std::vector<element> elems;
			elems.push_back (m_expr.val ());
			combination c (std::move (elems));
			vals.push_back (c);
			break;
			}
		case operation::plus:
			{
			sum_set set (m_locations.size ());
			while (!set.is_max ())
				{
				if (set.sum () == m_expr.val ())
					{
					vals.push_back (set.get ());
					}
				++set;
				}
			return vals;
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
	return vals;
	}
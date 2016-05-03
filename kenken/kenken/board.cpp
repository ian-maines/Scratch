// Kenken supporting types
// Copyright Ian Maines 4/30/16

#include "board.h"

board::board (const std::vector<math_group_ptr>& math_groups)
	: m_board (board_size*board_size)
	{
	_build (math_groups);
	}

void board::set_value (point point, element val)
	{
	m_board[_point_to_index(point)]->SetValue (val);
	}

std::string board::to_string ()
	{
	static_assert (board_size < std::numeric_limits<unsigned char>::max (), "Board size should not exceed 9, 255 will break this function.");
	std::string val;
	for (unsigned char y = 0; y < board_size; ++y)
		{
		for (unsigned char x = 0; x < board_size; ++x)
			{
			val += "[" + std::to_string ((m_board[_point_to_index(point (x, y))]->GetValue ())) + "]";
			if (board_size - 1 == x)
				{
				val += "\n";
				}
			}
		}
	return val;
	}

board_element_ptr& board::get (point pt)
	{
	return m_board[_point_to_index (pt)];
	}

void board::set (point pt, board_element_ptr&& val)
	{

	}

int board::_point_to_index (point pt) const
	{
	return (pt.row () * 9 + pt.col ());
	}

void board::_build (const std::vector<math_group_ptr>& mgs)
	{
	std::for_each (mgs.begin (), mgs.end (),
				   [this](auto mg) {
						std::for_each (mg->get_locations ().begin (), mg->get_locations ().end (),
									   [this, &mg](auto pt) {
											ASSERT (!m_board.at(_point_to_index (pt)));
											m_board[_point_to_index (pt)] = std::make_unique<board_element> (mg);
										});
				});
	unsigned int i (0);
	for (const auto& mg : mgs)
		{
		i += mg->get_combinations ().size ();
		DEBUG ("Math group %s\n", mg->to_string ().c_str ());
		}
	DEBUG ("Board contains %u locally valid combinations.\n", i);
	}
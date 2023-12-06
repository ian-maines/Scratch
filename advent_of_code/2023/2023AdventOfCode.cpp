// 2023AdventOfCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <format>

namespace
{
	void _debug (const std::string& str)
		{
		std::cout << str << std::endl;
		}

	std::vector<std::string> string_vec_input (std::string fname)
		{
		std::vector<std::string> input;
		std::ifstream f;
		f.open (fname, std::ifstream::in);
		if (!f.is_open ())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		std::string line;
		std::getline (f, line);
		while (!f.eof ())
			{
			input.push_back (line);
			std::getline (f, line);
			}
		return input;
		}

	std::vector<std::string> _get_delimited_vals (const std::string& str, char delim, bool bHasSpace = true)
		{
		std::vector<std::string> rv;
		size_t a = 0;
		auto b = str.find (delim);
		while (b != std::string::npos)
			{
			rv.push_back (str.substr (a, b - a));
			a = (b + (bHasSpace ? 2 : 1));
			b = str.find (delim, a);
			}
		rv.push_back (str.substr (a, b - a));
		return rv;
		}

	// Finds numbers separatd by anything that's not a digit and returns them in the order found.
	// Supports negative numbers
	std::vector<int> _get_all_numbers (const std::string& str)
		{
		static const std::string _numbers = "-0123456789";
		std::vector<int> rv;
		size_t next_pos = str.find_first_of (_numbers, 0);
		while (next_pos != std::string::npos)
			{
			if ((next_pos < str.length () - 1))
				{
				if (str[next_pos] == '-' && !isdigit (str[next_pos + 1]))
					{
					continue;
					}
				}

			rv.push_back (std::stoi (std::string (str.begin () + next_pos, str.end ())));
			if (str[next_pos] == '-')
				{
				++next_pos;
				}
			while (next_pos < str.length () && std::isdigit (str[next_pos]))
				{
				++next_pos;
				}
			next_pos = str.find_first_of (_numbers, next_pos);
			}
		return rv;
		}

	struct coord_t
		{
		coord_t () = default;
		coord_t (int _x, int _y)
			: x (_x), y (_y)
			{}
		int x = 0;
		int y = 0;

		size_t taxicab_distance (const coord_t& other) const
			{
			return (std::abs (x - other.x) + std::abs (y - other.y));
			}
		};

	std::string _print (int x, int y)
		{
		return std::format ("[{},{}]", x, y);
		}
	std::string _print (const coord_t& c)
		{
		return _print (c.x, c.y);
		}

	using grid_t = std::vector<std::vector<char>>;

	grid_t _reserve_grid (size_t x_dim, size_t y_dim, char fill)
		{
		return grid_t (x_dim, std::vector<char> (y_dim, fill));
		}

	void _set_grid (grid_t& g, coord_t c, char v)
		{
		g[c.x][c.y] = v;
		}

	void _print (const grid_t& g, int ystart = 0, int xstart = 0)
		{
		system ("cls");
		// Header row
		if (xstart != 0)
			{
			std::string hdr;
			int val = xstart;
			for (int _x = 0; _x < g.size (); ++_x)
				{
				hdr.append (std::format ("{}", val));
				++val;
				}
			_debug (hdr);
			}

		for (int y = 0; y < g[0].size (); ++y)
			{
			std::string row;
			row.append (std::format ("{:7} |", ystart + y));
			for (int x = 0; x < g.size (); ++x)
				{
				row.push_back (g[x][y]);
				}
			_debug (row);
			}
		}
}

namespace day1
{
std::vector<std::string> day1_load ()
	{
	const auto _fname =
		"d1p1.txt";
		//"d1p1_test.txt";

	return string_vec_input (_fname);
	}

std::vector<int> day1_input_parse (std::vector<std::string> input)
	{
	const std::string digits = { "0123456789" };
	std::vector<int> rv;
	
	// We should not have a vecotr of each line

	for (const auto l : input)
		{
		auto first_ch = std::find_first_of (l.begin (), l.end (), digits.begin (), digits.end ());
		auto last_ch = std::find_first_of (l.rbegin (), l.rend (), digits.begin (), digits.end ());
		
		// We SHOULD be able to find a digit in each line.
		if (first_ch != l.end () && last_ch != l.rend ())
			{
			rv.push_back (10 * (*first_ch - '0') + (*last_ch - '0'));
			}
		else
			{
			throw - 1;
			}
		}

	return rv;
	}

void problem1 ()
	{
	size_t val = 0;
	auto input = day1_input_parse (day1_load());

	for (const auto v : input)
		{
		std::cout << v << std::endl;
		val += v;
		}
	std::cout << "Sum of all calibration values is '" << val << "'" << std::endl;
	}

void problem2 ()
	{
	// Approach: Load each line from the file. Convert any text numbers to their corresponding digits. Then use problem 1 algo
	static const std::vector<std::pair<std::string, char>> _conversion =
		{
		 {"one", '1'}
		,{"two", '2'}
		,{"three",'3'}
		,{"four",'4'}
		,{"five",'5'}
		,{"six",'6'}
		,{"seven",'7'}
		,{"eight",'8'}
		,{"nine",'9'}
		,{"zero",'0'}
		};

	auto raw_input = day1_load ();

	std::vector<int> vals;
	for (auto& line : raw_input)
		{
		std::vector<int> this_line_vals;
		for (auto i = 0; i < line.length (); ++i)
			{
			if (isdigit (line[i]))
				{
				this_line_vals.push_back (line[i] - '0');
				}
			else
				{
				for (const auto number : _conversion)
					{
					auto pos = line.find (number.first, i);
					if (pos == i)
						{
						this_line_vals.push_back (number.second - '0');
						}
					}
				}
			}
		if (this_line_vals.size () < 1) { throw -2; }

		vals.push_back (10 * *this_line_vals.begin () + *(this_line_vals.end () - 1));
		}
		
	size_t val = 0;
	for (const auto v : vals)
		{
		std::cout << v << std::endl;
		val += v;
		}
	std::cout << "Sum of all calibration values is '" << val << "'" << std::endl;
	}
}

namespace day2
{
// We get a file with a game number followed by a comma-delimited list of counts of cubes and colors.
struct handful_t
	{
	size_t red = 0;
	size_t green = 0;
	size_t blue = 0;
	};

struct game_t
	{
	size_t ID = 0;
	std::vector<handful_t> handfuls;
	};

std::vector<game_t> day2_input ()
	{
	const auto _fname =
		"d2p1.txt";
		//"d2p2_test.txt";


	auto input = string_vec_input (_fname);

	std::vector<game_t> rv;
	// Now parse out each game.

	for (auto line : input)
		{
		game_t game;
		// For completeness we'll grab the game ID from the file.
		// This is inefficient but I've got better things to do and lots of CPU power
		game.ID = _get_all_numbers (line)[0];

		// THe line is "Game <ID>: " and after the colon and space the first handful starts. +2 so we don't include the space.
		std::string handfuls_substr (line, line.find (':') + 2);

		auto separated_handful_strs = _get_delimited_vals (handfuls_substr, ';', true);

		for (auto handful_str : separated_handful_strs)
			{
			handful_t this_handful;
			auto values = _get_delimited_vals (handful_str, ',', true);
			// At this point we should have a max of 3 strings that contain a number and a color. Parse.
			for (auto value : values)
				{
				// Again not efficient but I've got better stuff to do.
				size_t count = _get_all_numbers (value)[0];

				// Not the most elegant implementation but that's not why I'm here.
				if (value.find ("red") != value.npos)
					{
					this_handful.red = count;
					}
				else if (value.find ("green") != value.npos)
					{
					this_handful.green = count;
					}
				else if (value.find ("blue") != value.npos)
					{
					this_handful.blue = count;
					}
				else
					{
					throw - 1;
					}
				}
			game.handfuls.push_back (this_handful);
			}
		rv.push_back (game);
		}
	return rv;
	}

enum color_t
	{
	red = 0,
	green,
	blue
	};

bool _contains_more_than (const game_t& game, const size_t num, const color_t color)
	{
	for (const auto& handful : game.handfuls)
		{
		switch (color)
			{
				case red:
					if (handful.red > num) return true;
					break;

				case green:
					if (handful.green > num) return true;
					break;

				case blue:
					if (handful.blue > num) return true;
					break;

				default:
					throw - 2;
					break;
			}
		}
	// IF we get here and didn't find anything the answer is no
	return false;
	}

void problem1 ()
	{
	auto games = day2_input ();

	// Now that we have all of the games we need to sum up all that would be impossible if we ever saw more than
	// 12 red, 13 green, or 14 blue cubes.
	size_t sum_of_ids = 0;
	for (const auto game : games)
		{
		if (!_contains_more_than (game, 12, color_t::red)
		 && !_contains_more_than (game, 13, color_t::green)
		 && !_contains_more_than (game, 14, color_t::blue))
			{
			sum_of_ids += game.ID;
			}
		}
	std::cout << "Sum of all game IDs that don't match criteria = '" << sum_of_ids << "'" << std::endl;
	}

handful_t get_mins (const game_t& game)
	{
	handful_t rv;
	for (const auto& handful : game.handfuls)
		{
		if (handful.red > rv.red) rv.red = handful.red;
		if (handful.green > rv.green) rv.green = handful.green;
		if (handful.blue > rv.blue) rv.blue = handful.blue;
		}
	return rv;
	}

void problem2 ()
	{
	auto games = day2_input ();

	// Now we're looking for the minimum number of cubes that could have been used to play each game.
	// We multiply those together and the sum up their values.
	size_t sum_of_powers = 0;
	for (auto game : games)
		{
		auto mins = get_mins (game);
		size_t power = mins.red * mins.green * mins.blue;
		sum_of_powers += power;
		}

	std::cout << "Sum of all Game powers = '" << sum_of_powers << "'" << std::endl;
	}
}

int main()
	{
	day2::problem2 ();
	}
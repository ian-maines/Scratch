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

}

int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

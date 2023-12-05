// advent_of_code.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>

#include <thread>
#include <chrono>

namespace
	{
	void _debug(const std::string& str)
		{
		std::cout << str << std::endl;
		}

	std::vector<std::string> string_vec_input(std::string fname)
		{
		std::vector<std::string> input;
		std::ifstream f;
		f.open(fname, std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		std::string line;
		std::getline(f, line);
		while (!f.eof())
			{
			input.push_back(line);
			std::getline(f, line);
			}
		return input;
		}

	std::vector<std::string> _get_delimited_vals(const std::string& str, char delim, bool bHasSpace = true)
		{
		std::vector<std::string> rv;
		size_t a = 0;
		auto b = str.find(delim);
		while (b != std::string::npos)
			{
			rv.push_back(str.substr(a, b - a));
			a = (b + (bHasSpace ? 2 : 1));
			b = str.find(delim, a);
			}
		rv.push_back(str.substr(a, b - a));
		return rv;
		}

	// Finds numbers separatd by anything that's not a digit and returns them in the order found.
	// Supports negative numbers
	std::vector<int> _get_all_numbers(const std::string& str)
		{
		static const std::string _numbers = "-0123456789";
		std::vector<int> rv;
		size_t next_pos = str.find_first_of(_numbers, 0);
		while (next_pos != std::string::npos)
			{
			if ((next_pos < str.length() - 1))
				{
				if (str[next_pos] == '-' && !isdigit(str[next_pos + 1]))
					{
					continue;
					}
				}
				
			rv.push_back(std::stoi(std::string(str.begin() + next_pos, str.end())));
			if (str[next_pos] == '-')
				{
				++next_pos;
				}
			while (next_pos < str.length () && std::isdigit(str[next_pos]))
				{
				++next_pos;
				}
			next_pos = str.find_first_of(_numbers, next_pos);
			}
		return rv;
		}

	struct coord_t
		{
		coord_t() = default;
		coord_t(int _x, int _y)
			: x(_x), y(_y)
			{}
		int x = 0;
		int y = 0;

		size_t taxicab_distance(const coord_t& other) const
			{
			return (std::abs(x - other.x) + std::abs(y - other.y));
			}
		};

	std::string _print(int x, int y)
		{
		return std::format("[{},{}]", x, y);
		}
	std::string _print(const coord_t& c)
		{
		return _print(c.x, c.y);
		}

	using grid_t = std::vector<std::vector<char>>;

	grid_t _reserve_grid (size_t x_dim, size_t y_dim, char fill)
		{
		return grid_t(x_dim, std::vector<char>(y_dim, fill));
		}

	void _set_grid(grid_t& g, coord_t c, char v)
		{
		g[c.x][c.y] = v;
		}

	void _print(const grid_t& g, int ystart = 0, int xstart = 0)
		{
		system("cls");
		// Header row
		if (xstart != 0)
			{
			std::string hdr;
			int val = xstart;
			for (int _x = 0; _x < g.size(); ++_x)
				{
				hdr.append(std::format("{}", val));
				++val;
				}
			_debug(hdr);
			}

		for (int y = 0; y < g[0].size(); ++y)
			{
			std::string row;
			row.append(std::format("{:7} |", ystart + y));
			for (int x = 0; x < g.size(); ++x)
				{
				row.push_back(g[x][y]);
				}
			_debug(row);
			}
		}

	// We need a board that has one more space on either side of horizontal min/max, as well as one more vertical
	struct grid_dimensions_t
		{
		int xmin = 9999999;
		int xmax = 0;
		int ymin = 9999999;
		int ymax = 0;
		size_t x_dim = 0;
		size_t y_dim = 0;
		};

	void _update_dims (grid_dimensions_t& d, const coord_t& c)
		{
		if (c.x < d.xmin)
			{
			d.xmin = c.x;
			}
		if (c.x > d.xmax)
			{
			d.xmax = c.x;
			}
		if (c.y < d.ymin)
			{
			d.ymin = c.y;
			}
		if (c.y > d.ymax)
			{
			d.ymax = c.y;
			}
		// Add 1 because the max dimension will be an index we have to be able to reference
		d.x_dim = (d.xmax - d.xmin)+1;
		d.y_dim = (d.ymax - d.ymin)+1;

		// For each axis, if the grid spans across its 0 we need to add another index
		if (d.ymin < 0 && d.ymax > 0)
			{
			d.y_dim += 1;
			}
		if (d.xmin < 0 && d.xmax > 0)
			{
			d.x_dim += 1;
			}
		}

	void _expand_grid(grid_dimensions_t& d, size_t by)	// expansion will be +by in both +and - directions for x and y
		{
		_update_dims(d, coord_t(d.xmin - by,d.ymin));
		_update_dims(d, coord_t(d.xmax + by, d.ymin));
		_update_dims(d, coord_t(d.xmin, d.ymin - by));
		_update_dims(d, coord_t(d.xmin, d.ymax + by));
		}

	auto _xcalc = [](const grid_dimensions_t& g, int x)
		{
		return (x - g.xmin);
		};
	auto _ycalc = [](const grid_dimensions_t& g, int y)
		{
		return (y - g.ymin);
		};

	coord_t _ccalc(const grid_dimensions_t& g, const coord_t c)
		{
		return coord_t (_xcalc(g, c.x), _ycalc(g, c.y));
		}
	}

namespace day_1
	{
	std::vector<size_t> day1_input()
		{
		std::ifstream f;
		f.open("d1p1.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		// Grab the input. Each elf is separated by a blank line.
		std::vector<size_t> elf_total_calories;
		while (!f.eof())
			{
			std::string cur_line;
			std::getline(f, cur_line);
			std::size_t current_elf_total_cals(0);
			while (!cur_line.empty() && !f.eof())	// When we hit a blank line, log the current elf and move on to the next
				{
				current_elf_total_cals += std::stoi(cur_line);
				std::getline(f, cur_line);
				}
			elf_total_calories.push_back(current_elf_total_cals);
			}
		return elf_total_calories;
		}
	void problem1()
		{
		auto elf_total_calories = day1_input();
		// Now that we have all of the elves total, find the largest;
		// Could sort and then just grab largest but can shortcut today.
		std::size_t most_calories = 0;
		std::for_each(elf_total_calories.begin(), elf_total_calories.end(), [&most_calories](size_t& cur) { if (cur > most_calories) { most_calories = cur; }});
		}
	void problem2()
		{
		auto elf_total_calories = day1_input();
		// Now I guess they're going to make us sort. Need to do it in reverse
		std::sort(elf_total_calories.begin(), elf_total_calories.end(), [](auto l, auto r) { return l > r ? true : false; });
		auto itr = elf_total_calories.begin();
		std::size_t top_3_total = *itr + *(itr + 1) + *(itr + 2);
		}
	}
namespace day_2
	{
	bool validate(const std::string& in)
		{
		// Expect the string to be "[ABC] [XYZ]\r\n"
		return in.size() > 2 && std::string("ABC").contains(in[0]) && in[1] == ' ' && std::string("XYZ").contains(in[2]);
		}
	std::vector<std::pair<char, char>> day2_input()
		{
		std::ifstream f;
		f.open("d2p1.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		// Grab the input. Each elf is separated by a blank line.
		std::vector<std::pair<char, char>> all_rounds;
		std::string cur_line;
		std::getline(f, cur_line);
		while (!f.eof())
			{
			if (!validate(cur_line))
				{
				std::cout << "Error: Current line format invalid. Line = '" << cur_line << "'" << std::endl;
				throw (-999);
				}
			std::pair<char, char> val(std::make_pair(cur_line[0], cur_line[2]));

			all_rounds.push_back(val);
			std::getline(f, cur_line);
			}
		return all_rounds;
		}
	// A and X are Rock 1
	// B and Y are Paper 2
	// C and Z are Scissors 3
	// Loss is 0, draw is 3, win is 6
	std::size_t calc_score(const std::pair<char, char> round)
		{
		const std::map<const char, const size_t> values = { {'A',1},{'X',1},{'B',2},{'Y',2},{'C',3},{'Z',3} };
		const std::map<const std::string, size_t> outcome_scores = {
				{{"AX"},3},	// Rock-Rock draw
				{{"BY"},3},	// Paper-Paper draw
				{{"CZ"},3},	// Scissors-Scissors draw
				{{"AY"},6},	// Rock-Paper win
				{{"AZ"},0},	// Rock-Scissors loss
				{{"BX"},0},	// Paper-Rock loss
				{{"BZ"},6},	// Paper-Scissors win
				{{"CX"},6},	// Scissors-Rock win
				{{"CY"},0},	// Scissors-Paper loss
			};
		std::string tmp;
		tmp.push_back(round.first);
		tmp.push_back(round.second);
		return outcome_scores.find(tmp)->second + values.find(round.second)->second;
		}

	void problem1()
		{
		auto rounds = day2_input();
		size_t total_score = 0;
		std::for_each(rounds.begin(), rounds.end(), [&total_score](auto v) {total_score += calc_score(v); });
		}

	// For problem 2, XYZ are not the RPS we choose, but whether we win, loose, or draw.
	// X = loose, Y = draw, Z = win
	std::pair<char, char> choose_result(const std::pair<char, char> in)
		{
		// For these maps, .first is the opponent's play and .second is the play we need to make to get the desired outcome
		const std::map<char, char> _loose =	{ {'A','Z'},{'B','X'}, {'C','Y'} };
		const std::map<char, char> _draw =	{ {'A','X'}, {'B','Y'}, {'C','Z' } };
		const std::map<char, char> _win = { {'A','Y'}, {'B','Z'}, {'C','X'} };

		const std::map<char, const std::map<char, char>>& _result_chooser = { {'X',_loose}, {'Y',_draw}, {'Z',_win} };

		// The first value in the pair is whatever we got from the input.
		// For the second value, we'll use .second from the input to determine if we need to w/l/d, select the correct map based on that, and then find the play we would need to make based on the input.
		// No error checking here - we validate the input file and this is a low risk app.
		return std::make_pair(in.first, _result_chooser.find(in.second)->second.find(in.first)->second);
		}
	void problem2()
		{
		// For this one we just need 2 loops - first one to rejigger the thing we end up playing.
		const auto _in_info = day2_input();
		std::vector<std::pair<char, char>> updated_rounds;
		std::for_each(_in_info.begin(), _in_info.end(), [&updated_rounds](auto input) {updated_rounds.push_back(choose_result(input)); });

		// Now that we've remapped the data based on what we were instructed to do for each round, calculate.
		size_t total_score = 0;
		std::for_each(updated_rounds.begin(), updated_rounds.end(), [&total_score](auto v) {total_score += calc_score(v); });
		}
	}
namespace day_3
	{
	// Each element of the vector is one sack. Each component of the pair is one of the compartments in the sack
	using sack_contents_t = std::vector <std::pair <std::vector<char>, std::vector<char>>>;
	bool validate(const std::string& str)
		{
		// must be even & all chars must be letters
		bool bLetters = true;
		for (const auto c : str)
			{
			if (!isalpha(c))
				{
				bLetters = false;
				}
			}
		return str.length() % 2 == 0 && bLetters;
		}
	sack_contents_t day3_input()
		{
		std::ifstream f;
		f.open("d3p1.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		// Grab the input. Each elf is separated by a blank line.
		sack_contents_t all_sacks;
		std::string cur_line;
		std::getline(f, cur_line);
		while (!f.eof())
			{
			if (!validate(cur_line))
				{
				std::cout << "Error: Current line format invalid. Line = '" << cur_line << "'" << std::endl;
				throw (-999);
				}
			// Split the strings by compartment
			all_sacks.push_back(std::make_pair(std::vector<char>(cur_line.begin(), cur_line.begin() + cur_line.length() / 2), std::vector<char>(cur_line.begin() + (cur_line.length() / 2), cur_line.end())));
			std::getline(f, cur_line);
			}
		return all_sacks;
		}

	// a-z priorities 1-26, A-Z priorities 27-52
	// A = 65, a = 97 so formula for priority is [a-z]-96, [A-Z]-65+27
	size_t priority(const char& c)
		{
		// capital letters
		if (c < 'a')
			{
			return c - 65 + 27;
			}
		else
			{
			return c - 96;
			}
		}

	void problem1()
		{
		auto all_sacks = day3_input();

		// Need to figure out the common items for the compartments of each sack in the list, then add their priorities
		std::vector<std::set<char>> common_item_by_sack;
		std::for_each(all_sacks.begin(), all_sacks.end(), [&](std::pair<std::vector<char>, std::vector<char>>& sack)
			{
			// Use a set to find the unique items in Sack 1
			const std::set<char> unique_items_s1 (sack.first.begin(), sack.first.end());

			// Now figure out which items are duplicate
			std::set<char> dupes_in_this_sack;
			std::for_each(sack.second.begin(), sack.second.end(), [&](char val)
				{
				if (unique_items_s1.contains(val))
					{
					dupes_in_this_sack.insert(val);
					}
				});
			common_item_by_sack.push_back(dupes_in_this_sack);
			});

		// Now for each dupe get the priority and add it up.
		size_t priority_sum = 0;
		// We don't need the set here, but started with it because the problem wasn't clear that only one item type could be out of place. Not going to re-write,
		// so this code supports a future where more than one item could be in the wrong compartment
		std::for_each(common_item_by_sack.begin(), common_item_by_sack.end(), [&priority_sum](auto set)
			{
			for (const auto c : set)
				{
				priority_sum += priority(c);
				}
			});
		}

	std::string _print(const std::vector<char>& v)
		{
		return std::string(v.begin(), v.end());
		}

	// This is searching both compartments of ONE ELF'S SACK
	bool search_both(char sought, const std::vector<char>& s1, const std::vector<char>& s2)
		{
		std::cout << "seeking '" << sought << "' in " << _print(s1) << " | " << _print(s2) << std::endl;
		return std::find(s1.begin(), s1.end(), sought) != s1.end() || std::find(s2.begin(), s2.end(), sought) != s2.end();
		}
	const auto _lb = "***************************************************************************";
	char find_common(const sack_contents_t& group)
		{
		// Three sacks. Need to find what's common across all three
		auto elf1 = (group.begin());
		auto elf2 = (group.begin() + 1);
		auto elf3 = (group.begin() + 2);

		// Outer loop is elf1. Go through both compartmenbts of their sack
		std::cout << _lb << std::endl << "Searching new group" << std::endl << _print (elf1->first) << _print(elf1->second) << " | " << _print(elf2->first) << _print(elf2->second) << " | " << _print(elf3->first) << _print(elf3->second) << std::endl << _lb << std::endl;
		for (const auto ch : elf1->first)
			{
			// See if any of their stuff is in either of the other elve's stuff
			if (search_both(ch, elf2->first, elf2->second) && search_both(ch, elf3->first, elf3->second))
				{
				std::cout << "found '" << ch << "'" << std::endl;
				return ch;
				}
			}
		for (const auto ch : elf1->second)
			{
			// See if any of their stuff is in either of the other elve's stuff
			if (search_both(ch, elf2->first, elf2->second) && search_both(ch, elf3->first, elf3->second))
				{
				std::cout << "found '" << ch << "'" << std::endl;
				return ch;
				}
			}
		throw (-9999);
		return 0;
		}

	void problem2()
		{
		// This time we need to grab everything but clump them into groups.
		// Each outer vector is a group. Inner vector is the three elves in the group's stuff
		std::vector<sack_contents_t> elf_groups_and_their_stuff;
		auto all_sacks = day3_input();

		// Now break it up by groups
		if (all_sacks.size() % 3 != 0)
			{
			std::cout << "NOT DIVISIBLE BY 3" << std::endl;
			throw (-99);
			}

		auto itr = all_sacks.begin();
		while (itr != all_sacks.end())
			{
			sack_contents_t cs;
			cs.push_back(*(itr++));
			cs.push_back(*(itr++));
			cs.push_back(*(itr++));
			elf_groups_and_their_stuff.push_back(cs);
			}

		// Now for each group need to find the common type
		std::vector<char> common_type_by_group;
		std::for_each(elf_groups_and_their_stuff.begin(), elf_groups_and_their_stuff.end(), [&common_type_by_group](auto& group)
			{
			common_type_by_group.push_back(find_common(group));
			});

		// Now for each dupe get the priority and add it up.
		size_t priority_sum = 0;
		// We don't need the set here, but started with it because the problem wasn't clear that only one item type could be out of place. Not going to re-write,
		// so this code supports a future where more than one item could be in the wrong compartment
		std::for_each(common_type_by_group.begin(), common_type_by_group.end(), [&priority_sum](char ch)
			{
			priority_sum += priority(ch);
			});
		}
	}
namespace day_4
	{
	// Data structure for what we get from the file.
	struct elf_input_t
		{
		elf_input_t(size_t s, size_t e) : range_start(s), range_end(e) {}
		size_t range_start = 0;
		size_t range_end = 0;
		};
	using elf_assignment_t = std::vector<size_t>;
	using elf_pair_t = std::pair<elf_assignment_t, elf_assignment_t>;
	using elf_input_pair_t = std::pair< elf_input_t, elf_input_t>;
	using input_pair_vector_t = std::vector<elf_input_pair_t>;
	using pair_vector_t = std::vector<std::pair<elf_assignment_t, elf_assignment_t>>;


	bool validate(const std::string& line)
		{
		auto itr = line.begin();
		if (!isdigit(*itr)) { return false; }		// Digit should start the sequence
		while (isdigit(*itr)) { itr++; }			// Go through the digits until we run out, expect a dash
		if (*itr++ != '-') { return false; }
		if (!isdigit(*itr)) { return false; }		// After that we should get another digit. Don't advance. That's what the loop is for
		while (isdigit(*itr)) { itr++; }					// Go through the digits until we run out, expect a comma
		if (*itr++ != ',') { return false; }
		if (!isdigit(*itr)) { return false; }		// Expect another digit
		while (isdigit(*itr)) { itr++; }					// Go through the digits until we run out, expect a dash
		if (*itr++ != '-') { return false; }
		if (!isdigit(*itr)) { return false; }		// Expect digits to end out the string
		while (line.end() != itr && isdigit(*itr)) { itr++; }
		if (itr != line.end()) { return false; }
		return true;
		}

	elf_input_pair_t parse_line(const std::string& line)
		{
		// Line should be a number, a dash, another number, a comma, a number, a dask, and another number.
		// Grab the info for the first elf
		size_t cur_pos = 0;
		size_t next_pos = line.find_first_not_of("0123456789", 0);

		size_t elf1b = std::stoi(std::string(line.begin() + cur_pos, line.begin() + next_pos));
		// skip the dash
		cur_pos = next_pos + 1;
		next_pos = line.find_first_not_of("0123456789", cur_pos);
		size_t elf1e = std::stoi(std::string(line.begin() + cur_pos, line.begin() + next_pos));
		// Skip the comma to grab the info for the 2nd elf
		cur_pos = next_pos + 1;
		next_pos = line.find_first_not_of("0123456789", cur_pos);
		size_t elf2b = std::stoi(std::string(line.begin() + cur_pos, line.begin() + next_pos));
		// skip the dash
		cur_pos = next_pos + 1;
		next_pos = line.find_first_not_of("0123456789", cur_pos);
		// We validated that the last number rounds out the string, so just go to the end.
		size_t elf2e = std::stoi(std::string(line.begin() + cur_pos, line.end()));



		return std::make_pair(elf_input_t(elf1b, elf1e), elf_input_t(elf2b, elf2e));
		}

	input_pair_vector_t day4_input()
		{
		std::ifstream f;
		f.open("d4p1.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		input_pair_vector_t rv;

		// Each line contains a range for two elves
		std::string cur_line;
		std::getline(f, cur_line);
		while (!f.eof())
			{
			if (!validate(cur_line))
				{
				std::cout << "invalid line '" << cur_line << "'" << std::endl;
				throw (-999);
				}

			rv.push_back(parse_line(cur_line));

			std::getline(f, cur_line);
			}

		return rv;
		}

	std::vector<size_t> vec_from_range(size_t b, size_t e)
		{
		std::vector<size_t> rv;

		for (size_t i = b; i <= e; ++i)
			{
			rv.push_back(i);
			}
		return rv;
		}

	void problem1()
		{
		auto input = day4_input();

		size_t pairs_fully_containing_other = 0;

		for (const auto& ip : input)
			{
			if ((ip.first.range_start <= ip.second.range_start && ip.first.range_end >= ip.second.range_end)
				|| ip.second.range_start <= ip.first.range_start && ip.second.range_end >= ip.first.range_end)
				{
				pairs_fully_containing_other++;
				}
			}

		// Now convert everything to ranges.
		// Wait maybe we'll care about this for p2
		pair_vector_t elf_range_pairs;

		for (const auto& ip : input)
			{
			elf_range_pairs.push_back(elf_pair_t(std::make_pair(vec_from_range(ip.first.range_start, ip.first.range_end), vec_from_range(ip.second.range_start, ip.second.range_end))));
			}
		}
	void problem2()
		{
		auto input = day4_input();

		size_t pairs_overlapping = 0;

		for (const auto& ip : input)
			{
			if (ip.first.range_start >= ip.second.range_start && ip.first.range_start <= ip.second.range_end
				|| ip.second.range_start >= ip.first.range_start && ip.second.range_start <= ip.first.range_end)
				{
				pairs_overlapping++;
				}
			}
		}
	}
namespace day_5
	{
	// Input for the day is in 2 parts. State of all of the stacks, and moving instructions.
	// Using a map because stacks are not 0-indexed. Makes things easier.
	using stack_state_t = std::map<size_t,std::stack<char>>;
	struct instruction_t
		{
		size_t src_stack = 0;
		size_t dst_stack = 0;
		size_t count = 0;
		};
	struct input_data_t
		{
		input_data_t(stack_state_t&& s, std::vector<instruction_t> i) : state(s), instructions (i) {}
		stack_state_t state;
		std::vector<instruction_t> instructions;
		
		};

	bool validate(const std::string& line)
		{

		return true;
		}

	void parse_state_line(const std::string& line, stack_state_t& state)
		{
		// For each line, we will have a space between each stack. The "contents" of the stack is either an uppercase letter in brackets or a space indicating no item.
		// Zero-indexed list of the location in the string where an item (uppercase letter) lives - or a space if this stack has no item at this level
		const std::vector<size_t> _columns = { 1,5,9,13,17,21,25,29,33 };
		if (line.length() < *(_columns.end() - 1))
			{
			std::cout << "line of insufficient length" << std::endl;
			throw (-99999);
			}

		for (size_t stack = 1; stack <= _columns.size(); stack++)
			{
			if (line.at(_columns.at(stack-1)) != ' ')
				{
				state.at(stack).push(line.at(_columns.at(stack - 1)));
				}
			}
		}

	instruction_t parse_instruction_line(const std::string& line)
		{
		const std::string _digits("0123456789");
		instruction_t rv;
		auto itr = std::find (line.begin(), line.end(), ' ');
		auto next_space = std::find(itr+1, line.end(), ' ');
		rv.count = std::stoi(std::string(std::find_first_of(itr, line.end(), _digits.begin(), _digits.end()), next_space));
		itr = next_space;
		next_space = std::find(itr+1, line.end(), ' ');
		itr = next_space;
		next_space = std::find(itr + 1, line.end(), ' ');
		rv.src_stack = std::stoi(std::string(std::find_first_of(itr, line.end(), _digits.begin(), _digits.end()), next_space));
		itr = next_space;
		next_space = std::find(itr+1, line.end(), ' ');
		itr = next_space;
		next_space = std::find(itr + 1, line.end(), ' ');
		rv.dst_stack = std::stoi(std::string(std::find_first_of(itr, line.end(), _digits.begin(), _digits.end()), next_space));
		return rv;
		}

	input_data_t day5_input()
		{
		// pre-populate. There are 9 stacks
		stack_state_t stacks{ {1,{} }, {2,{}}, {3,{}}, {4,{}}, {5,{}}, {6,{}}, {7,{}}, {8,{}}, {9,{}} };
		std::vector<instruction_t> instructions;

		std::ifstream f;
		f.open("d5p1.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		// Each line contains a range for two elves
		std::string cur_line;
		std::getline(f, cur_line);
		bool bProcInitialState = true;
		while (!f.eof())
			{
			if (!validate(cur_line))
				{
				std::cout << "invalid line '" << cur_line << "'" << std::endl;
				throw (-999);
				}

			// After the initial state, we get one line of column numbers followed by a blank line
			if (bProcInitialState && cur_line.length() == 0)
				{
				std::getline(f, cur_line);
				bProcInitialState = false;
				continue;
				}
			if (bProcInitialState && !cur_line.contains('['))
				{
				std::getline(f, cur_line);
				continue;
				}

			if (bProcInitialState)
				{
				parse_state_line(cur_line, stacks);
				}
			else
				{
				instructions.push_back (parse_instruction_line(cur_line));
				}

			std::getline(f, cur_line);
			}

		// Re-arrange the stacks so they're in the right order
		stack_state_t ordered_stacks;
		for (auto& stack : stacks)
			{
			std::queue<char> holding_area;
			std::stack<char> ordered;
			while (stack.second.size() > 0)
				{
				holding_area.push(stack.second.top());
				stack.second.pop();
				}
			while (holding_area.size() > 0)
				{
				ordered.push(holding_area.front());
				holding_area.pop();
				}
			ordered_stacks.insert(std::make_pair(stack.first, ordered));
			}

		return input_data_t (std::move(ordered_stacks), std::move(instructions));
		}

	void problem1()
		{
		auto in_data = day5_input();

		// Now process all of the movement.

		for (const auto& instruction : in_data.instructions)
			{
			for (int i = 0; i < instruction.count; ++i)
				{
				in_data.state[instruction.dst_stack].push(in_data.state[instruction.src_stack].top());
				in_data.state[instruction.src_stack].pop();
				}
			}
		// Now grab the top element of each stack.
		std::string final_result;
		for (const auto& state : in_data.state)
			{
			final_result.push_back(state.second.top());
			}
		}

	void problem2()
		{
		auto in_data = day5_input();

		// Now process all of the movement.
		for (const auto& instruction : in_data.instructions)
			{
			// For this problem, to maintain the correct order we need to have a holding area.
			// We could re-do the data structures we're using on input to eliminate this need, but not going to rearchitect for a one-day problem
			std::stack<char> holding_area;
			for (int i = 0; i < instruction.count; ++i)
				{
				holding_area.push(in_data.state[instruction.src_stack].top());
				in_data.state[instruction.src_stack].pop();
				}
			while (holding_area.size())
				{
				in_data.state[instruction.dst_stack].push(holding_area.top());
				holding_area.pop();
				}
			}
		// Now grab the top element of each stack.
		std::string final_result;
		for (const auto& state : in_data.state)
			{
			final_result.push_back(state.second.top());
			}
		}
	}
namespace day_6
{
	bool validate(const std::string& line)
		{
		for (const auto c : line)
			{
			if (!std::islower(c))
				{
				return false;
				}
			}
		return true;
		}

	std::string day6_input()
		{
		std::ifstream f;
		f.open("d6p1.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		std::string data;
		std::getline(f, data);
		if (!validate(data))
			{
			std::cout << "Invalid line '" << data << "'" << std::endl;
			}

		return data;
		}

	size_t problem1()
		{
		// Need to find the location of the first 4 in a row that are not the same.

		auto in_data = day6_input();

		// Would be cool to use a data structure here (eg a queue) but queues don't provide access to middle elements to be able to check for duplicates so we'll just do it the manual way.
		const size_t _lookback = 14;
		size_t i = _lookback-1;
		for (; i < in_data.length(); ++i)
			{
			std::set<char> last_count;
			bool bFound = true;
			for (size_t j = _lookback - 1; j < _lookback; --j)	// Unsigned rollover dependence feels yucky
				{
				if (!(last_count.insert(in_data[i-j]).second))
					{
					bFound = false;
					break;
					}
				}

			if (bFound)
				{
				return i + 1;
				}

			}
		return -1;
		}
}
namespace day_7
	{
	// C++ doesn't have a tree structure, so here's our own.
	struct fs_item_t
		{
		// file constructor
		fs_item_t(size_t s, std::string n, fs_item_t& p) : bIsFile (true), file_size(s), name (n), parent (p) {}
		// Directory constructor
		fs_item_t(std::string n, fs_item_t& p) : bIsFile(false), name (n), parent(p) {}
		// Root constructor
		fs_item_t(bool bRoot, std::string n) : bIsFile(false), name(n), parent(*this) {}

		bool bIsFile = false;
		size_t file_size = 0;
		std::string name;
		fs_item_t& parent;
		std::vector<fs_item_t> child_items;
		std::size_t size_of_all = 0;

		// cd.. at root just returns root. No error.
		fs_item_t& go_up()
			{
			if (name == "/")
				{
				return *this;
				}
			return parent;
			}
		};

	std::pair<size_t, std::string> parse_cd_down(const std::string& line)
		{
		// file size or dir name starts at index 5
		// Digit indicates a file size thus a file
		if (std::isdigit(line[0]))
			{
			size_t number_end_pos = 0;
			while (isdigit(line[number_end_pos]))
				{
				number_end_pos++;
				}
			std::string number_part(line, 0, number_end_pos);
			size_t file_size = std::stoi(number_part);
;			std::string name(line, number_end_pos);
			return std::make_pair(file_size, name);
			}
		// Otherwise just a dir.
		std::string name (line, 4);
		return std::make_pair(0, name);
		}

	size_t populate_sizes(fs_item_t& tgt)
		{
		for (auto& child : tgt.child_items)
			{
			if (child.bIsFile)
				{
				tgt.size_of_all += child.file_size;
				}
			else
				{
				tgt.size_of_all += populate_sizes(child);
				}
			}
		return tgt.size_of_all;
		}

	void find_dirs_lt(std::vector<fs_item_t>& found, fs_item_t& level, const size_t max_size)
		{
		if (level.size_of_all <= max_size)
			{
			found.push_back(level);
			}
		for (auto& dir : level.child_items)
			{
			if (!dir.bIsFile)
				{
				find_dirs_lt(found, dir, max_size);
				}
			}
		return;
		}

	void search_all(fs_item_t** found, size_t needed_space, fs_item_t& current_level)
		{
		if (current_level.name.compare("dzbfsf") == 0)
			{
			std::cout << "asdf" << std::endl;
			}
		if (current_level.size_of_all < (*found)->size_of_all && current_level.size_of_all >= needed_space)
			{
			*found = &current_level;
			}
		for (auto& dir : current_level.child_items)
			{
			if (!dir.bIsFile)
				{
				search_all(found, needed_space, dir);
				}
			}
		}

	void problem1()
		{
		fs_item_t root (true, "/");
		// Grab the contents of the file
		std::vector<std::string> input = string_vec_input("d7p1.txt");

		// Have to do some stuff with pointers here. Not super safe. Good thing this isn't a production application.
		fs_item_t* current = &root;
		
		// Now populate our tree
		for (const auto& line : input)
			{
			if (line.contains("$ cd .."))
				{
				current = &(current->go_up());
				}
			else if (line.contains("$ cd /")) { current = &root; }
			else if (line.contains("$ cd "))
				{
				const std::string target_dir(line, 5);
				std::vector<fs_item_t>::iterator itr = std::find_if(current->child_items.begin(), current->child_items.end(), [&target_dir](auto item) 
					{
					return (!item.bIsFile) && (target_dir.compare(item.name) == 0);
					});
				// We're just going to assume our input file is valida and let the fire start here if it's not.
				fs_item_t& i = *itr;
				current = &i;
				}
			else if (line.contains("$ ls"))
				{
				//no-op - just move on.
				}
			else
				{
				// Any other line is the definition of a new file or directory
				auto thing(parse_cd_down(line));
				if (thing.first == 0)
					{
					// directory
					fs_item_t item(thing.second, *current);
					current->child_items.push_back(item);
					}
				else
					{
					// File
					fs_item_t item(thing.first, thing.second, *current);
					current->child_items.push_back(item);
					}
				}
			}
		// Now we need to do a depth-first search for dirs containing <= 100000 bytes
		size_t used_space = populate_sizes(root);

		// Problem 1
		{
		size_t total_size_100k = 0;
		std::vector<fs_item_t> found;
		find_dirs_lt(found, root, 100000);

		for (auto dir : found)
			{
			total_size_100k += dir.size_of_all;
			}
		}

		// Problem 2
		{
		fs_item_t* found = &root;
		size_t space_needed = 30000000 - (70000000 - root.size_of_all);
		search_all (&found, space_needed, root);
		std::cout << found->size_of_all << std::endl;
		}
		}
	}
namespace day_8
{
	// a vector of rows, so outer vector is the Y coordinate, inner is X. Example below - items in a grid showing their [x,y] coords
	// [0,0],[1,0] 
	// [0,1],[1,1]

	// Using vector access
	// vec[0][0], vec[0][1]
	// vec[1][0], vec[1][1]
	using tree_grid_t = std::vector<std::vector<size_t>>;

	tree_grid_t day8_input()
		{
		tree_grid_t rv;

		std::ifstream f;
		f.open("d8p1.txt", std::ifstream::in);
		//f.open("d8p1_test.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		std::string line;
		std::getline(f, line);
		while (!f.eof())
			{
			std::vector<size_t> row;
			for (char ch : line)
				{
				// There is probably a better way to do this conversion
				row.push_back(ch - size_t('0'));
				}
			rv.push_back(row);
			std::getline(f, line);
			}

		return rv;
		}

	// Takes two coordinates and determines if the tree is visible
	bool is_visible(const tree_grid_t& grid, size_t x, size_t y)
		{
		// Easy case: Tree is on the outside of the grid
		if (x == 0 || y == 0 || x == (grid[y].size() - 1) || y == (grid.size() - 1))
			{
			return true;
			}

		// If not, we have to do some real work.
		// Evaluate the row and column the tree are in.

		// If any tree in all directions is as tall as (or taller) than the current tree, the tree is not visible

		// Column first
		bool bVisibleAbove = true;
		// Start "above" the current tree
		for (int i = 0; i < y; i++)
			{
			if (grid[i][x] >= grid[y][x])
				{
				bVisibleAbove = false;
				}
			}
		// Now do below the current tree
		bool bVisibleBelow = true;
		for (int i = y+1; i < grid[y].size(); i++)
			{
			if (grid[i][x] >= grid[y][x])
				{
				bVisibleBelow = false;
				}
			}

		// now rows
		// Start to the left
		bool bVisibleLeft = true;
		for (int i = 0; i < x; i++)
			{
			if (grid[y][i] >= grid[y][x])
				{
				bVisibleLeft = false;
				}
			}
		// Now to the right
		bool bVisibleRight = true;
		for (int i = x+1; i < grid.size(); i++)
				{
				if (grid[y][i] >= grid[y][x])
					{
					bVisibleRight = false;
					}
				}

		bool bIsVisible = (bVisibleAbove || bVisibleBelow || bVisibleLeft || bVisibleRight);
		std::cout << "tree at [" << y << "][" << x << "] with height " << grid[y][x] << " is " << (bIsVisible ? "visible" : "hidden") << std::endl;;
		return bIsVisible;
		}

	size_t calc_visibility_score(const tree_grid_t& grid, size_t x, size_t y)
		{
		// Easy case: Tree is on the outside of the grid. Score 0
		if (x == 0 || y == 0 || x == (grid[y].size() - 1) || y == (grid.size() - 1))
			{
			return 0;
			}

		// Column first
		size_t view_above = 0;
		// Start at the tree and work up (negative)
		for (int i = y-1; i >= 0; --i)
			{
			// If the tree is taller, it blocks our view and we can stop this loop
			view_above++;
			if (grid[i][x] >= grid[y][x])
				{
				break;
				}
			}
		// Now do below the current tree
		size_t view_below = 0;
		// Start at the tree and work down (positive)
		for (int i = y + 1; i < grid[y].size(); i++)
			{
			// If the tree is taller, it blocks our view and we can stop this loop
			view_below++;
			if (grid[i][x] >= grid[y][x])
				{
				break;
				}
			}

		// now rows
		// Start to the left
		size_t view_left = 0;
		// Start at the tree and work left (negative)
		for (int i = x-1; i >= 0; --i)
			{
			// If the tree is taller, it blocks our view and we can stop this loop
			view_left++;
			if (grid[y][i] >= grid[y][x])
				{
				break;
				}
			}
		// Now to the right
		size_t view_right = 0;
		for (int i = x + 1; i < grid.size(); i++)
			{
			// If the tree is taller, it blocks our view and we can stop this loop
			view_right++;
			if (grid[y][i] >= grid[y][x])
				{
				break;
				}
			}

		size_t score = view_above * view_below * view_left * view_right;
		std::cout << "tree at [" << y << "][" << x << "] with height " << grid[y][x] << " has score " << score << std::endl;;
		return score;
		}

	void problem1()
		{
		auto grid = day8_input();

		size_t visible_trees = 0;
		for (int y = 0; y < grid.size(); ++y)
			{
			for (int x = 0; x < grid[y].size(); ++x)
				{
				if (is_visible(grid, x, y))
					{
					visible_trees++;
					}
				}
			}
		}
	void problem2()
		{
		auto grid = day8_input();

		size_t current_max_score = 0;
		for (int y = 0; y < grid.size(); ++y)
			{
			for (int x = 0; x < grid[y].size(); ++x)
				{
				size_t cs = calc_visibility_score(grid, x, y);
				if (cs > current_max_score)
					{
					current_max_score = cs;
					}
				}
			}
		}
}
namespace day_9
	{
	// Data structures for input
	enum direction_t
		{
		up = 'U',
		down = 'D',
		left = 'L',
		right = 'R'
		};
	struct instruction_t
		{
		direction_t d;
		size_t count;
		};

	std::vector<instruction_t> day9_input()
		{
		std::vector<instruction_t> rv;

		std::ifstream f;
		f.open("d9p1.txt", std::ifstream::in);
		//f.open("d9p1_test.txt", std::ifstream::in);
		//f.open("d9p2_test.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		std::string line;
		std::getline(f, line);
		while (!f.eof())
			{
			instruction_t i;
			i.d = direction_t(line[0]);
			// Magic number 2 below is because an input line is always one direction character, a space, and then a number. e.g. "U 23"
			i.count = std::stoi(std::string(line, 2, std::string::npos));
			rv.push_back(i);
			std::getline(f, line);
			}
		return rv;
		}

	struct coordinate_t
		{
		int16_t x = 0;
		int16_t y = 0;

		bool operator== (const coordinate_t& rhs) const
			{
			return x == rhs.x && y == rhs.y;
			}
		};
	
	void perform_step(const direction_t d, coordinate_t& h_pos)
		{
		switch (d)
			{
			case direction_t::up:
				h_pos.y++;
				break;
			case direction_t::down:
				h_pos.y--;
				break;
			case direction_t::left:
				h_pos.x--;
				break;
			case direction_t::right:
				h_pos.x++;
				break;
			}
		}

	// The coordinate returned is the x and y moves the tail would need to make to be in the same space as the head
	// *H
	// T*
	// In the above example, T would need to move +1X and +1Y to be in the same spot, so 1,1 is returned
	coordinate_t diff_pos(coordinate_t& h_pos, coordinate_t& t_pos)
		{
		coordinate_t rv;
		rv.x = h_pos.x - t_pos.x;
		rv.y = h_pos.y - t_pos.y;
		return rv;
		}

	void dec_magnitude(int16_t& v)
		{
		// Decrement magnitude
		if (v > 0)
			{
			v--;
			}
		else if (v < 0)
			{
			v++;
			}
		else
			{
			// wut
			throw (-8);
			}
		}

	coordinate_t calc_move_p1(const coordinate_t& c)
		{
		coordinate_t rv = c;
		// This move (rv) if applied to c, should result in either x or y of c being 0

		if (c.x != 0 && c.y != 0)
			{
			// diagonal move. Only decrement the larger of the two differences.
			if (std::abs(c.x) > std::abs(c.y))
				{
				dec_magnitude(rv.x);
				}
			else
				{
				dec_magnitude(rv.y);
				}
			}
		else if (c.x == 0)
			{
			// just need to move on the Y axis
			dec_magnitude(rv.y);
			}
		else if (c.y == 0)
			{
			// Just need to move on the x axis
			dec_magnitude(rv.x);
			}
		else
			{
			// wut
			throw (-7);
			}
		return rv;
		}

	void problem1()
		{
		auto instructions = day9_input();
		std::vector<coordinate_t> visited_spaces;

		coordinate_t h_pos;
		coordinate_t t_pos;

		visited_spaces.push_back(t_pos);

		for (const auto& i : instructions)
			{
			for (int j = 0; j < i.count; ++j)
				{
				perform_step(i.d, h_pos);

				auto diff = diff_pos(h_pos, t_pos);

				if (std::abs(diff.x) <= 1 && std::abs(diff.y) <= 1)
					{
					// No move necessary
					continue;
					}
				else
					{
					auto move = calc_move_p1(diff);
					// Apply the move to the tail
					t_pos.x += move.x;
					t_pos.y += move.y;
					std::cout << "[" << t_pos.x << "][" << t_pos.y << "]" << std::endl;
					if (std::find_if(visited_spaces.begin(), visited_spaces.end(), [&t_pos](const coordinate_t& pos) { return t_pos.x == pos.x && t_pos.y == pos.y; }) == visited_spaces.end())
						{
						visited_spaces.push_back(t_pos);
						}
					else
						{
						std::cout << "rejected duplicate " << "[" << t_pos.x << "][" << t_pos.y << "]" << std::endl;
						}
					}
				}
			}
		}

	void visualize(std::vector<coordinate_t> state)
		{
		// Largest the board could ever be is 10x10. We'll go 12x12 for some borders
		constexpr int  __size = 12;
		std::vector<std::vector<char>> board (__size, std::vector<char>(__size, '*'));

		// We need to normalize the x,y values for our 12x12 board.
		// The smallest numbers will just become referenced as 0 later
		int x_min = 99999999;
		int y_min = 99999999;
		for (const auto& p : state)
			{
			if (p.x < x_min) { x_min = p.x; }
			if (p.y < y_min) { y_min = p.y; }
			}

		for (int x = x_min; x < x_min+__size; ++x)
			{
			for (int y = y_min; y <y_min+__size; ++y)
				{
				for (int i = 0; i < state.size(); ++i)
					{
					if (state[i].x == x && state[i].y == y)
						{
						board[y-y_min][x-x_min] = i + '0';
						}
					}
				}
			}
		// Because the console wrties top-bottom we have to reverse the inner vectors
		std::for_each(board.rbegin(), board.rend(), [](auto& vec_char)
			{
			for (auto ch : vec_char)
				{
				std::cout << ch;
				}
			std::cout << std::endl;
			});
		}

	// Problem 2 allows diagonal ending positions
	coordinate_t calc_move_p2(const coordinate_t& c)
		{
		coordinate_t rv = c;
		// This move (rv) if applied to c, should result in either x or y of c being 0

		if (c.x != 0 && c.y != 0)
			{
			// If any move direction is greater than 1, just move diagonally one
			if (std::abs(rv.x) > 1 || std::abs(rv.y) > 1)
				{
				// diagonal move. Just move one in each direction.
				rv.x = (rv.x > 0 ? 1 : -1);
				rv.y = (rv.y > 0 ? 1 : -1);
				}
			else
				{
				// diagonal move. Only decrement the larger of the two differences.
				if (std::abs(c.x) > std::abs(c.y))
					{
					dec_magnitude(rv.x);
					}
				else
					{
					dec_magnitude(rv.y);
					}
				}
			}
		else if (c.x == 0)
			{
			// just need to move on the Y axis
			dec_magnitude(rv.y);
			}
		else if (c.y == 0)
			{
			// Just need to move on the x axis
			dec_magnitude(rv.x);
			}
		else
			{
			// wut
			throw (-7);
			}
		return rv;
		}

	void problem2()
		{
		auto instructions = day9_input();
		std::vector<coordinate_t> visited_spaces;

		std::vector<coordinate_t> rope(10); // H = rope[0], T = rope[9]

		visited_spaces.push_back(rope[9]);

		for (const auto& i : instructions)
			{
			std::cout << "== " << (char(i.d)) << i.count << " ==" << std::endl;
			for (int j = 0; j < i.count; ++j)
				{
				perform_step(i.d, rope[0]);
				
				// Now for each remaining knot in the rope, calc the move:
				for (int i = 1; i < rope.size(); ++i)
					{
					coordinate_t& puller = rope[i - 1];
					coordinate_t& pulled = rope[i];
					auto diff = diff_pos(puller, pulled);

					if (std::abs(diff.x) <= 1 && std::abs(diff.y) <= 1)
						{
						// No move necessary
						continue;
						}
					else
						{
						auto move = calc_move_p2(diff);
						// Apply the move to the tail
						pulled.x += move.x;
						pulled.y += move.y;
						}
					}

				system("cls");
				visualize(rope);
				std::cout << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));

				// Now that we've updated the whole rope, tail state.
				//std::cout << "[" << rope[9].x << "][" << rope[9].y << "]" << std::endl;
				if (std::find_if(visited_spaces.begin(), visited_spaces.end(), [&rope](const coordinate_t& pos) { return rope[9].x == pos.x && rope[9].y == pos.y; }) == visited_spaces.end())
					{
					visited_spaces.push_back(rope[9]);
					}
				else
					{
					//std::cout << "rejected duplicate " << "[" << rope[9].x << "][" << rope[9].y << "]" << std::endl;
					}
				}
			}
		}
	}
namespace day_10
	{
	bool cycle_to_check(size_t cycle)
		{
		// On the 20th cycle and every 40 cycles after
		if (cycle >= 20 && (cycle == 20 || (cycle - 20) % 40 == 0))
			{
			return true;
			}
		return false;
		}

	void problem1()
		{
		auto input = string_vec_input("d10p1.txt");
		//auto input = string_vec_input("d10p1_test.txt");

		size_t current_cycle = 1;
		int x_reg_val = 1;

		size_t sig_str_sum = 0;
		for (const auto& line : input)
			{
			// On the 20th cycle and every 40 cycles after
			if (cycle_to_check(current_cycle))
				{
				sig_str_sum += current_cycle * x_reg_val;
				}

			current_cycle++;

			if (line.contains("noop"))
				{
				}
			else
				{
				int val = std::stoi(std::string(line, 5));
				if (cycle_to_check(current_cycle))
					{
					sig_str_sum += current_cycle * x_reg_val;
					}
				x_reg_val += val;
				current_cycle++;
				}
			}
		}

	void problem2()
		{
		auto input = string_vec_input("d10p1.txt");

		size_t reg_val = 1;
		auto itr = input.begin();
		int val = 0;
		for (size_t cycle = 0; /*will break*/; ++cycle)
			{
			size_t crt_pos = cycle % 40;
			if (crt_pos == 0)
				{
				std::cout << std::endl;
				}

			// First draw the pixel
			if (std::abs(int(reg_val) - int(crt_pos)) <= 1)
				{
				std::cout << "#";
				}
			else
				{
				std::cout << ".";
				}

			// Do cycle stuff
			if ((*itr).contains("noop"))
				{
				++itr;
				}
			else
				{
				// start the addx op
				if (val == 0)
					{
					val = std::stoi(std::string(*itr, 5));
					}
				// finish the addx op
				else
					{
					reg_val += val;
					val = 0;
					++itr;
					}
				}
			}
		}

	}
namespace day_11
	{
	enum operation
		{
		plus = 0,
		mult
		};

	const std::vector<size_t> primes_we_care_about = { 2,3,5,7,11,13,17,19,23 };

	// The key is the prime number (modularity) we're after, the second is the current mod value of the starting value after all operations.
	using divisibility_data_t = std::map<size_t, size_t>;

	struct item_t
		{
		item_t(size_t v) : value(v), mvalue (v)
			{
			for (const auto& val : primes_we_care_about)
				{
				m_div.insert(std::make_pair(val, v % val));
				}
			}
		const size_t value;
		size_t mvalue;

		void process(operation o, size_t v)
			{
			if (o == operation::plus)
				{
				if (v == 0)
					{
					throw (-6);
					}
				mvalue += v;
				}
			else
				{
				if (v == 0)
					{
					mvalue = value * value;
					}
				else
					{
					mvalue = value * v;
					}
				}
			}

		void process_p2(operation o, size_t v)
			{
			if (o == operation::plus)
				{
				if (v == 0)
					{
					throw (-6);
					}
				for (auto & p : m_div)
					{
					p.second += (v % p.first);
					}
				}
			else
				{
				for (auto& p : m_div)
					{
					if (v == 0)
						{
						// For squaring it what do we do? Should this have no impact?
						p.second = ((p.second) * (p.second)) % p.first;
						}
					else
						{
						p.second = ((p.second * (v % p.first)) % p.first);
						}
					}
				}
			}
		divisibility_data_t m_div;
		};


	struct monkey_t
		{
		std::queue<item_t> items;
		operation op;
		// If operand is 0 the operand is the old value (so op == mult && operand == 0 means new = old * old)
		size_t operand;
		size_t test_divisor;

		size_t true_target_monkey;
		size_t false_target_monkey;

		// Total number of items this monkey has inspected
		size_t inspection_count = 0;

		void process_items(std::vector<monkey_t>& monkeys, bool bWorryDivisor = true)
			{
			while (items.size())
				{
				item_t i = items.front();
				i.process(op, operand);
				if (bWorryDivisor)
					{
					i.mvalue = i.mvalue / 3;
					}

				size_t t_monkey = 0;
				if (i.mvalue % test_divisor == 0)
					{
					t_monkey = true_target_monkey;
					}
				else
					{
					t_monkey = false_target_monkey;
					}

				monkeys[t_monkey].items.push(i);
				items.pop();
				inspection_count++;
				}
			}
		void process_items_p2(std::vector<monkey_t>& monkeys)
			{
			while (items.size())
				{
				item_t i = items.front();
				i.process_p2(op, operand);

				size_t t_monkey = 0;
				if (i.m_div[test_divisor] == 0)
					{
					t_monkey = true_target_monkey;
					}
				else
					{
					t_monkey = false_target_monkey;
					}

				monkeys[t_monkey].items.push(i);
				items.pop();
				inspection_count++;
				}
			}
		};

	std::string _print(monkey_t m)	// Copy so we can get the items
		{
		std::string str;
		while  (m.items.size())
			{
			auto i = m.items.front();
			str.append(std::format("{},", i.value));
			m.items.pop();
			}
		return str;
		}

	std::string _print(const std::vector<monkey_t>& monkeys)
		{
		std::string str;
		size_t i = 0;
		for (const auto& m : monkeys)
			{
			str.append(std::format("monkey{}: {}: {}\r\n",i, m.inspection_count ,_print(m)));
			++i;
			}
		return str;
		}

	std::vector<monkey_t> day11_input(const std::string& _fname)
		{
		auto lines = string_vec_input(_fname);

		std::vector<monkey_t> rv;

		std::vector<std::string>::iterator itr = lines.begin();
		while (itr != lines.end())
			{
			itr++;	// skip monkey #
			monkey_t m;
			// First get the items
			auto vals = _get_delimited_vals(std::string(itr->begin() + 18, itr->end()), ',');
			for (const auto& v : vals)
				{
				m.items.push(item_t(std::stoi(v)));
				}
			// Now get the operation stuf
			++itr;
			operation o = operation::plus;
			char c = (*(itr->begin() + 23));
			if (c == '*')
				{
				o = operation::mult;
				}
			m.op = o;
			size_t operand = 0;	// 0 means self, so if not a digit, stay 0
			c = *(itr->begin() + 25);
			if (isdigit(c))
				{
				operand = std::stoi(std::string(itr->begin() + 25, itr->end()));
				}
			m.operand = operand;

			// Now get the test divisor
			++itr;
			m.test_divisor = std::stoi(std::string(itr->begin() + 21, itr->end()));
			// Now the the target monkeys
			++itr;
			m.true_target_monkey = std::stoi(std::string(itr->begin() + 29, itr->end()));
			++itr;
			m.false_target_monkey = std::stoi(std::string(itr->begin() + 30, itr->end()));

			// Advance to the next/blank line
			++itr;
			if (itr != lines.end())
				{
				// skip it
				++itr;
				}
			rv.push_back(m);
			}

		return rv;
		}


	void problem1()
		{
		std::string _fname =
			//"d11p1_test.txt";
			"d11p1.txt";
		auto monkeys = day11_input(_fname);
		_debug(_print(monkeys));
		for (int i = 0; i < 20; ++i)
			{
			for (auto& m : monkeys)
				{
				m.process_items(monkeys);
				if (i % 100 == 0)
					{
					_debug(_print(monkeys));
					}
				}
			}
		_debug(_print(monkeys));
		}
	void problem2()
		{
		std::string _fname =
			"d11p1_test.txt";
			//"d11p1.txt";
		auto monkeys = day11_input(_fname);
		auto monkeys2 = monkeys;
		_debug(_print(monkeys));
		for (int i = 0; i < 20; ++i)
			{
			for (int i = 0; i < monkeys.size(); i++)
				{
				monkeys[i].process_items_p2(monkeys);
				monkeys2[i].process_items(monkeys2, false);
				_debug(_print(monkeys));
				_debug(_print(monkeys2));
				_debug("\r\n");
				}
			//if (i % 100 == 0)
				{
				_debug(_print(monkeys));
				}
			}
		_debug(_print(monkeys));
		}
	}
namespace day_12
	{
	// First vector is x axis, 2nd is y so vec[1][4] = coordinates 1,4. 0,0 is the top right
	struct grid_t
		{
		using _t = std::vector<std::vector<char>>;
		using _coord = std::pair<int, int>;

		char get(size_t x, size_t y) const { return grid[x][y]; }
		void set(_coord& c, char ch) { grid[c.first][c.second] = ch; }

		char get(_coord& c) const
			{
			return get(c.first, c.second);
			}

		void print() const
			{

			for (size_t y = 0; y < grid[0].size(); ++y)
				{
				for (size_t x = 0; x < grid.size(); ++x)
					{
					std::cout << grid[x][y];
					}
				std::cout << std::endl;
				}
			std::cout << std::endl;
			}

		_coord start = std::make_pair(0, 0);
		_coord end = std::make_pair(0, 0);

		_t grid;
		};

	std::string _print(int x, int y)
		{
		return std::format("[{},{}]", x, y);
		}
	std::string _print(const grid_t::_coord& c)
		{
		return _print(c.first, c.second);
		}

	grid_t _for_display;
	size_t _cycles = 0;

	grid_t day12_input()
		{
		grid_t rv;

		std::ifstream f;
		f.open("d12p1.txt", std::ifstream::in);
		//f.open("d12p1_test.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		std::string line;
		std::getline(f, line);
		while (!f.eof())
			{
			std::vector<char> row;
			for (char ch : line)
				{
				// There is probably a better way to do this conversion
				row.push_back(ch);
				}
			rv.grid.push_back(row);
			std::getline(f, line);
			}

		// Need to swap X and Y so the outer vector is columns instead of rows
		grid_t::_t swapped;
		for (int i = 0; i < rv.grid[0].size(); ++i)
			{
			std::vector<char> col;
			for (size_t j = 0; j < rv.grid.size(); ++j)
				{
				col.push_back(rv.grid[j][i]);
				}
			swapped.push_back(col);
			}

		rv.grid = swapped;

		for (int x = 0; x < rv.grid.size(); ++x)
			{
			for (int y = 0; y < rv.grid[x].size(); ++y)
				{
				if (rv.get(x, y) == 'S')
					{
					rv.start = std::make_pair(x, y);
					}
				else if (rv.get(x, y) == 'E')
					{
					rv.end = std::make_pair(x, y);
					}
				}
			}

		std::cout << "begin=" << rv.get(rv.start) << " end=" << rv.get(rv.end) << std::endl;

		rv.set(rv.start, 'a');
		rv.set(rv.end, 'z');

		rv.print();

		return rv;
		}

	struct coord_t
		{
		coord_t(int _x, int _y)
			: x(_x), y(_y)
			{}
		int x = 0;
		int y = 0;
		};

	bool _valid(const grid_t& g, const coord_t& c)
		{
		return c.x < g.grid.size() && c.y < g.grid[0].size();
		}

	bool is_valid_move(const grid_t& g, const coord_t& src, const coord_t& dst)
		{
		//std::cout << "d12_debug: checking move to " << _print(x, y) << std::endl;
		// Grid boundaries check
		if (dst.x >= g.grid.size() || dst.y >= g.grid[0].size() || dst.x < 0 || dst.y < 0)
			{
			//std::cout << "d12_debug: rejecting move from " << _print(cur_loc) << " to " << _print(x, y) << " due to grid boundary" << std::endl;
			return false;
			}

		// Can only increment by max of one letter.
		if (g.get(dst.x, dst.y) - g.get(src.x, src.y) > 1)
			{
			//std::cout << "d12_debug: rejecting move from " << _print(src.x, src.y) << g.get(src.x,src.y) << " to " << _print(dst.x, dst.y) << g.get(dst.x, dst.y) << " due to height difference" << std::endl;
			return false;
			}

		return true;
		}

	using move_ct_t = std::vector<std::vector<int64_t>>;
	void _print(const move_ct_t& ct)
		{
		for (int y = 0; y < ct[0].size(); ++y)
			{
			for (int x = 0; x < ct.size(); ++x)
				{
				if (ct[x][y] == -1)
					{
					std::cout << "XXX ";
					}
				else
					{
					std::cout << std::setfill('0') << std::setw(3) << ct[x][y] << " ";
					}
				}
			std::cout << std::endl;
			}
		std::cout << std::endl;
		}

	void problem1()
		{
		auto grid = day12_input();

		size_t min_moves = 9999999999;

		move_ct_t moves_to_loc = { grid.grid.size(), std::vector<int64_t>(grid.grid[0].size(), -1) };

		moves_to_loc[grid.start.first][grid.start.second] = 0;

		//_print(moves_to_loc);

		bool bSomethingChanged = true;
		while (bSomethingChanged)
			{
			bSomethingChanged = false;
			for (int x = 0; x < grid.grid.size(); ++x)
				{
				for (int y = 0; y < grid.grid[0].size(); ++y)
					{
					const coord_t src(x, y);
					const std::vector<coord_t> moves = { {0,-1}, {0,1}, {-1,0}, {1,0} };
					// Look on each side of us to see if there is data already there. 
					for (auto& m : moves)
						{
						coord_t dst(x + m.x, y + m.y);
						if (_valid(grid, dst))
							{
							if (is_valid_move(grid, src, dst))
								{
								// Update the other path if we have a value and the other path does not OR ours is smaller
								if (moves_to_loc[src.x][src.y] != -1)
									{
									if (moves_to_loc[dst.x][dst.y] == -1)
										{
										bSomethingChanged = true;
										moves_to_loc[dst.x][dst.y] = moves_to_loc[src.x][src.y] + 1;
										}
									// Update this path if we and the other quare have a path, but the other is shorter
									else if (moves_to_loc[dst.x][dst.y] > (moves_to_loc[src.x][src.y] +1))
										{
										bSomethingChanged = true;
										moves_to_loc[dst.x][dst.y] = moves_to_loc[src.x][src.y] + 1;
										}
									}
								}
							}
						}
					}
				}
			//_print(moves_to_loc);
			}
		std::cout << "Number of moves to reach dst is " << moves_to_loc[grid.end.first][grid.end.second];
		}
	void problem2()
		{
		auto grid = day12_input();

		size_t min_moves = 9999999999;


		//_print(moves_to_loc);

		std::vector<coord_t> starting_positions;
		for (int x = 0; x < grid.grid.size(); ++x)
			{
			for (int y = 0; y < grid.grid[0].size(); ++y)
				{
				if (grid.get(x, y) == 'a')
					{
					starting_positions.push_back(coord_t(x, y));
					}
				}
			}

		for (const auto& c : starting_positions)
			{
			move_ct_t moves_to_loc = { grid.grid.size(), std::vector<int64_t>(grid.grid[0].size(), -1) };

			moves_to_loc[c.x][c.y] = 0;
			bool bSomethingChanged = true;
			while (bSomethingChanged)
				{
				bSomethingChanged = false;
				for (int x = 0; x < grid.grid.size(); ++x)
					{
					for (int y = 0; y < grid.grid[0].size(); ++y)
						{
						const coord_t src(x, y);
						const std::vector<coord_t> moves = { {0,-1}, {0,1}, {-1,0}, {1,0} };
						// Look on each side of us to see if there is data already there. 
						for (auto& m : moves)
							{
							coord_t dst(x + m.x, y + m.y);
							if (_valid(grid, dst))
								{
								if (is_valid_move(grid, src, dst))
									{
									// Update the other path if we have a value and the other path does not OR ours is smaller
									if (moves_to_loc[src.x][src.y] != -1)
										{
										if (moves_to_loc[dst.x][dst.y] == -1)
											{
											bSomethingChanged = true;
											moves_to_loc[dst.x][dst.y] = moves_to_loc[src.x][src.y] + 1;
											}
										// Update this path if we and the other quare have a path, but the other is shorter
										else if (moves_to_loc[dst.x][dst.y] > (moves_to_loc[src.x][src.y] + 1))
											{
											bSomethingChanged = true;
											moves_to_loc[dst.x][dst.y] = moves_to_loc[src.x][src.y] + 1;
											}
										}
									}
								}
							}
						}
					}
				//_print(moves_to_loc);
				}
			//std::cout << "Number of moves to reach dst is " << moves_to_loc[grid.end.first][grid.end.second] << std::endl;
			if (moves_to_loc[grid.end.first][grid.end.second] > 0 && moves_to_loc[grid.end.first][grid.end.second] < min_moves)
				{
				min_moves = moves_to_loc[grid.end.first][grid.end.second];
				}
			}
		std::cout << "min_moves = " << min_moves << std::endl;
		}
	}
namespace day_13
	{
	// Nested structure that can contain more of itself. Each element is either a single element or another list.
	// bIsElement will tell you which.
	struct list_t
		{
		//element ctor
		list_t(size_t val)
			: bIsElement (true)
			, element (val)
			{}
		// list ctor
		list_t(list_t* p)
			: parent (p)
			{}

		bool bIsElement = false;
		size_t element = 0;
		std::vector<list_t> _l;
		list_t* parent = nullptr;
		};
	using input_t = std::vector<std::pair<list_t, list_t>>;

	list_t packet_from_string(const std::string& str)
		{
		list_t rv (nullptr);
		auto itr = str.begin();
		list_t* current = &rv;
		// Expect the first char to be '['
		if (*itr++ != '[')
			{
			throw (-5);
			}
		while (itr != str.end())
			{
			if (*itr == '[')
				{
				current->_l.push_back(list_t(current));
				current = &current->_l.back();
				}
			else if (*itr == ']')
				{
				// Move back up one level if we're at the end of this list
				if (current->parent == nullptr)
					{
					//std::cout << "Reached the end of the packet." << std::endl;
					return rv;
					}
				current = current->parent;	// nullptr concerns here but if we have valid input data shouldn't be a problem
				}
			else if (*itr == ',')	// Nothing to do here, just move on - a new '[' or digit will do that for us.
				{
				}
			else if (std::isdigit(*itr))
				{
				// Can have multi-digit numbers
				auto digits_end = itr+1;
				while (isdigit(*digits_end))
					{
					digits_end++;
					}
				size_t val = std::stoi(std::string(itr, digits_end));
				current->_l.push_back(list_t(val));
				itr = digits_end;
				continue;
				}
			else
				{
				// Wut
				throw (-6);
				}
			itr++;
			}
		return rv;
		}

	std::string _print(const list_t& l)
		{
		if (l.bIsElement)
			{
			return std::format("{}", l.element);
			}
		std::string str;
		str.append("[");
		for (auto itr = l._l.begin(); itr != l._l.end(); ++itr)
			{
			if (itr->bIsElement)
				{
				str.append(std::format("{}",itr->element));
				}
			else
				{
				str.append(_print(*itr));
				}
			if ((itr + 1) != l._l.end())
				{
				str.append(",");
				}
			}
		str.append("]");
		return str;
		}

	// Returns true if the order is correct, false otherwise
	enum result
		{
		good = 0,
		bad,
		ind	//indeterminant
		};
	result evaluate(const list_t& A, const list_t& B)
		{
		_debug(std::format("Evaluating\r\n{}\r\n{}", _print(A), _print(B)));
		if (A.bIsElement && B.bIsElement)
			{
			if (A.element < B.element)
				{
				_debug(std::format("{}<{}, pass", A.element, B.element));
				// We're good as soon as we hit this condition once.
				return result::good;
				}
			if (A.element > B.element)
				{
				_debug(std::format("{}>{}, fail", A.element, B.element));
				// We're bad as soon as we hit this condition once.
				return result::bad;
				}
			}
		else if (!(A.bIsElement) && !(B.bIsElement))
			{
			auto sA = A._l.begin();
			auto sB = B._l.begin();
			while (true)
				{
				// handle empty case
				if (sA == A._l.end())
					{
					// if A runs out first, order is correct
					if (sB != B._l.end())
						{
						_debug(std::format("List A ran out first, good"));
						return result::good;
						}
					break;
					}
				else if (sB == B._l.end())
					{
					// B ran out first, so fail
					_debug(std::format("List B ran out first, failure"));
					return result::bad;
					}
				// If we haven't reached the end, we need to compare
				else
					{
					auto r = evaluate(*sA, *sB);
					if (r != result::ind)
						{
						return r;
						}
					}
				// Otherwise move on to the next
				sA++;
				sB++;
				}
			// If we get through the whole loop with no result, ind
			return result::ind;
			}
		// Only one of the elements is a list
		else
			{
			if (A.bIsElement)
				{
				list_t nA(A.parent);	// Hack - probably no parent?
				nA._l.push_back(list_t(A.element));
				return evaluate(nA, B);
				}
			else
				{
				list_t nB(B.parent);	// Hack - probably no parent?
				nB._l.push_back(list_t(B.element));
				return evaluate(A, nB);
				}
			}
		return result::ind;
		}

	input_t day13_input ()
		{
		input_t rv;
		std::ifstream f;
		//f.open("d13p1.txt", std::ifstream::in);
		f.open("d13p2.txt", std::ifstream::in);
		//f.open("d13p1_test.txt", std::ifstream::in);
		if (!f.is_open())
			{
			std::cout << "ERROR: Input file not opened!" << std::endl;
			throw (-9);
			}

		std::string line;
		std::getline(f, line);
		while (!f.eof())
			{
			// Input is in pairs of lines separated by blank line,
			list_t A = packet_from_string(line);
			std::getline(f, line);
			list_t B = packet_from_string(line);
			std::pair<list_t, list_t> packet_pair = std::make_pair(A, B);
			std::getline(f, line);
			// Expect this line to be empty
			if (line.size() != 0)
				{
				throw (- 7);
				}
			_debug(_print(A));
			std::cout << std::endl;
			_debug(_print(B));
			std::cout << std::endl;
			std::cout << std::endl;
			rv.push_back(packet_pair);
			std::getline(f, line);
			}
		return rv;
		}

	void problem1()
		{
		auto in = day13_input();

		size_t good_packet_sum = 0;

		for (int i = 0; i < in.size(); ++i)
			{
			std::cout << std::endl << "*******************************************************************" << std::endl;
			if (evaluate(in[i].first, in[i].second) == result::good)
				{
				// 1-indexed for some reason
				good_packet_sum += (i+1);
				}
			}
		}

	void problem2()
		{
		auto in = day13_input();
		// Now we just need a vector without the pairs.
		std::vector<list_t> all;
		for (auto& p : in)
			{
			all.push_back(p.first);
			all.push_back(p.second);
			}

		std::sort(all.begin(), all.end(), [](const auto& l, const auto& r) {return (evaluate(l, r) != result::bad); });

		std::cout << std::endl << "*******************************************************************" << std::endl;
		std::cout << std::endl << "*******************************************************************" << std::endl;
		std::cout << std::endl << "*******************************************************************" << std::endl;
		size_t i = 1;
		for (const auto& p : all)
			{
			std::cout << (std::format("{}|{}", i++, _print(p))) << std::endl;;
			}
		}
	}
namespace day_14
{

using path_t = std::vector<coord_t>;

using input_data_t = std::vector<path_t>;

input_data_t day14_input()
	{
	const std::string _fname =
		"d14p1.txt";
		//"d14p1_test.txt";
	auto strs = string_vec_input(_fname);

	input_data_t rv;

	for (const auto& l : strs)
		{
		std::string::const_iterator c_begin = l.begin();
		auto offset = l.find("->", (c_begin - l.begin()));

		path_t this_path;
		while (true)
			{
			std::string substr (c_begin, l.begin() + offset);
			// Now we have two numbers separated by commas
			size_t cur_pos = 0;
			size_t next_pos = substr.find_first_not_of("0123456789", 0);

			int x = std::stoi(std::string(substr.begin() + cur_pos, substr.begin() + next_pos));
			cur_pos = next_pos + 1;
			next_pos = substr.find_first_not_of("0123456789", cur_pos);
			int y = std::stoi(std::string(substr.begin() + cur_pos, next_pos != std::string::npos ? substr.begin() + next_pos : substr.end()));
			this_path.push_back(coord_t(x, y));

			if (offset >= l.size())
				{
				break;
				}

			c_begin = l.begin() + offset + 3;	// 3 for the " -> " so we can skip over it (we start on the space
			offset = l.find("->", (c_begin - l.begin()));
			if (offset == std::string::npos)
				{
				offset = l.end() - l.begin();
				}
			}
		rv.push_back(this_path);
		}
	return rv;
	}


void problem1()
	{
	auto in = day14_input();

	// We need a board that has one more space on either side of horizontal min/max, as well as one more vertical
	int xmin = 500;
	int xmax = 500;
	int ymax = 0;
	for (const auto p : in)
		{
		for (const auto& c : p)
			{
			if (c.x < xmin)
				{
				xmin = c.x;
				}
			if (c.x > xmax)
				{
				xmax = c.x;
				}
			if (c.y > ymax)
				{
				ymax = c.y;
				}
			}
		}

	if (500 - xmin > xmax - 500)	// if the negative range is bigger, use it for dims
		{
		xmax = (500 - xmin) + 500;
		}
	else
		{
		xmin = 500 - (xmax - 500);
		}
	const size_t x_dim = (xmax - xmin) + 2;

	const size_t y_dim = ymax + 1;
	auto _xcalc = [&xmin](int x)
		{
		// In our input data X is centered at 500. 
		// So x = 500 should result in 500 - xmin + 1
		return (x - xmin) + 1;
		};
	grid_t grid = grid_t(x_dim, std::vector<char>(y_dim, '.'));
	const auto _sand_src = coord_t(_xcalc(500), 0);
	_set_grid(grid, _sand_src, '+');

	// Populate the grid with the paths
	for (const auto& p : in)
		{
		for (int i = 0; i + 1 < p.size(); ++i)
			{
			coord_t c = p[i];
			coord_t n = p[i + 1];

			int x_move = n.x - c.x;
			int y_move = n.y - c.y;

			// Assuming based on the description we can't have diagonals
			if (x_move && y_move)
				{
				throw (-5);
				}
			if (x_move)
				{
				int dir = x_move > 0 ? 1 : -1;
				while (c.x != n.x)
					{
					_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
					c.x += dir;
					}
				_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
				}
			else
				{
				int dir = y_move > 0 ? 1 : -1;
				while (c.y != n.y)
					{
					_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
					c.y += dir;
					}
				_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
				}
			}
		_print(grid);
		}

	// Now that we have the initialized grid, simulate falling sand
	bool bAbyss = false;
	size_t grains_of_sand = 0;
	while (!bAbyss)
		{
		grains_of_sand++;
		bool bCanMove = true;
		coord_t s = _sand_src;
		while (bCanMove)
			{
			// Check boundaries
			if (s.x == 0 || s.x == grid.size() - 1 || s.y == grid[0].size())
				{
				bAbyss = true;
				bCanMove = false;
				// This grain never came to rest
				grains_of_sand--;
				break;
				}
			if (grid[s.x][s.y + 1] == '.')
				{
				s.y++;
				}
			else if (grid[s.x - 1][s.y + 1] == '.')
				{
				s.y++;
				s.x--;
				}
			else if (grid[s.x + 1][s.y + 1] == '.')
				{
				s.y++;
				s.x++;
				}
			else
				{
				// If we can't move, print the sand and get out of here
				bCanMove = false;
				grid[s.x][s.y] = 'o';
				}
			}
		_print(grid);
		}
	}

void problem2()
	{
	auto in = day14_input();

	// We need a board that has one more space on either side of horizontal min/max, as well as one more vertical
	int xmin = 500;
	int xmax = 500;
	int ymax = 0;
	for (const auto p : in)
		{
		for (const auto& c : p)
			{
			if (c.x < xmin)
				{
				xmin = c.x;
				}
			if (c.x > xmax)
				{
				xmax = c.x;
				}
			if (c.y > ymax)
				{
				ymax = c.y;
				}
			}
		}

	xmin -= 100;
	xmax += 100;

	if (500 - xmin > xmax - 500)	// if the negative range is bigger, use it for dims
		{
		xmax = (500 - xmin) + 500;
		}
	else
		{
		xmin = 500 - (xmax - 500);
		}
	const size_t x_dim = (xmax - xmin) + 2;

	const size_t y_dim = ymax + 1 + 2;
	auto _xcalc = [&xmin](int x)
		{
		// In our input data X is centered at 500. 
		// So x = 500 should result in 500 - xmin + 1
		return (x - xmin) + 1;
		};
	grid_t grid = grid_t(x_dim, std::vector<char>(y_dim, '.'));
	const auto _sand_src = coord_t(_xcalc(500), 0);
	_set_grid(grid, _sand_src, '+');

	// Populate the grid with the paths
	for (const auto& p : in)
		{
		for (int i = 0; i + 1 < p.size(); ++i)
			{
			coord_t c = p[i];
			coord_t n = p[i + 1];

			int x_move = n.x - c.x;
			int y_move = n.y - c.y;

			// Assuming based on the description we can't have diagonals
			if (x_move && y_move)
				{
				throw (-5);
				}
			if (x_move)
				{
				int dir = x_move > 0 ? 1 : -1;
				while (c.x != n.x)
					{
					_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
					c.x += dir;
					}
				_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
				}
			else
				{
				int dir = y_move > 0 ? 1 : -1;
				while (c.y != n.y)
					{
					_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
					c.y += dir;
					}
				_set_grid(grid, coord_t(_xcalc(c.x), c.y), '#');
				}
			}
		}

	// Now populate the floor
	for (int x = 0; x < grid.size(); ++x)
		{
		_set_grid(grid,coord_t(x, grid[x].size() - 1), '#');
		}
	_print(grid);

	// Now that we have the initialized grid, simulate falling sand
	bool bStopped = false;
	size_t grains_of_sand = 0;
	size_t cycle_counter = 0;
	while (!bStopped)
		{
		++cycle_counter;
		if (cycle_counter % 10000 == 0)
			{
			_print(grid);
			}
		grains_of_sand++;
		bool bCanMove = true;
		coord_t s = _sand_src;
		while (bCanMove)
			{
			// Check boundaries
			if (s.x == 0 || s.x == grid.size() - 1 || s.y == grid[0].size())
				{
				_print(grid);
				throw (-7);		// Don't expect to reach the boundary
				}
			if (grid[s.x][s.y + 1] == '.')
				{
				s.y++;
				}
			else if (grid[s.x - 1][s.y + 1] == '.')
				{
				s.y++;
				s.x--;
				}
			else if (grid[s.x + 1][s.y + 1] == '.')
				{
				s.y++;
				s.x++;
				}
			else
				{
				// If we can't move, print the sand and get out of here
				bCanMove = false;
				grid[s.x][s.y] = 'o';
				}
			}	// while (bCanMove)

		if (grid[_sand_src.x][_sand_src.y] == 'o')
			{
			bStopped = true;
			_print(grid);
			break;
			}
		}
	std::cout << "sand = " << grains_of_sand << std::endl;
	}
}
namespace day_15
	{
	struct sensor_t
		{
		sensor_t (const coord_t& l, const coord_t& n)
			: loc (l), nb (n), td_radius (loc.taxicab_distance(nb))
			{}

		sensor_t(const std::string& line)
			{
			auto vals = _get_all_numbers(line);
			if (vals.size() != 4)
				{
				throw (-11);
				}
			loc.x = vals[0];
			loc.y = vals[1];
			nb.x = vals[2];
			nb.y = vals[3];
			td_radius = loc.taxicab_distance(nb);
			}

		std::string print() const
			{
			return std::format("Sensor at {} nearest beacon {} tc distance {}", _print(loc), _print(nb), td_radius);
			}

		coord_t loc;		// Sensor location (absolute)
		coord_t nb;			// Nearest Beacon location (absolute)
		size_t td_radius;	// Radius of this senor.
		};

	std::vector<sensor_t> day15_input(const std::string& _fname)
		{
		auto strs = string_vec_input(_fname);

		std::vector<sensor_t> rv;

		for (const auto& l : strs)
			{
			rv.push_back(sensor_t(l));
			}

		return rv;
		}

	void problem1()
		{
		const auto _fname =
			//"d15p1.txt";
			"d15p1_test.txt";
		auto sensors = day15_input(_fname);

		grid_dimensions_t dims;

		for (const auto& s : sensors)
			{
			_update_dims(dims, s.loc);
			_update_dims(dims, s.nb);
			}


		constexpr bool bDoTheFastWay = false;
		if (bDoTheFastWay)
			{
			size_t open_spaces = 0;
			auto _go = [](bool& bOutputhere, char c) { if (!bOutputhere) { bOutputhere = true; /*std::cout << c;*/ }};
			int max_x = std::abs(dims.xmax) + std::abs(dims.xmin) + 10;
			// Commented out for loop for the smaller test data
			for (int y = -10; y < std::abs(dims.ymax) + std::abs(dims.ymin) + 10; ++y)
				//for (int y = 2000000; y < 2000000+1; ++y)
				{
				// Commented out for loop for smaller test data
				for (int x = -10; x < max_x; ++x)
					//for (int x = -max_x*2; x < max_x*2; ++x)
					{
					// For each value of X at this y coordinate, we need to know if a beacon is or could be present.
					coord_t here(x, y);
					bool bOutputHere = false;
					for (auto s : sensors)
						{
						if (here.x == s.nb.x && here.y == s.nb.y)
							{
							_go(bOutputHere, 'B');
							}
						else if (here.x == s.loc.x && here.y == s.loc.y)
							{
							_go(bOutputHere, 'S');
							bOutputHere = true;
							}
						else if (here.taxicab_distance(s.loc) <= s.loc.taxicab_distance(s.nb))
							{
							// Don't count the beacon location
							if (here.x != s.nb.x || here.y != s.nb.y)
								{
								if (!bOutputHere && y == 2000000) { open_spaces++; }
								_go(bOutputHere, '#');
								}
							}
						}
					_go(bOutputHere, '.');
					}
				std::cout << std::endl;
				}
			}


		_expand_grid(dims, 10);

		grid_t grid = _reserve_grid(dims.x_dim, dims.y_dim, '.');

		for (const auto& s : sensors)
			{
			_set_grid(grid, _ccalc(dims,s.loc), 'S');
			_set_grid(grid, _ccalc(dims,s.nb), 'B');
			}

		_print(grid, dims.ymin);
		// Now go through for each sensor and mark off where beacons can NOT be
		for (const auto& s : sensors)
			{
			auto tcd = s.loc.taxicab_distance(s.nb);
			// We need to find all cordinates on the grid with a taxicab distance LT the above
			// There is probably a spart way to do this but I don't care about time complexity to iterate the whole thing
			for (int x = 0; x < dims.x_dim; ++x)
				{
				for (int y = 0; y < dims.y_dim; ++y)
					{
					// x,y here will be in 0-based coords for the grid, but s.loc and s.nb are in full grid coords. Convert.
					auto this_distance = coord_t(x, y).taxicab_distance(_ccalc(dims, s.loc));
					if (this_distance <= tcd)
						{
						if (grid[x][y] == '.')
							{
							_set_grid(grid, coord_t(x, y), '#');
							}
						}
					}
				}
			_print(grid, dims.ymin);
			}
		_print(grid, dims.ymin);

		}

// Returns true if the specified coordinate is within the radius of any sensor or out of range
constexpr size_t _4mil = 4'000'000;
bool check_other_sensors(const std::vector<sensor_t>& sensors, const coord_t& c)
	{
	if (c.x < 0 || c.x > _4mil || c.y < 0 || c.y > _4mil)
		{
		return true;
		}
	for (const auto& s : sensors)
		{
		if (s.loc.taxicab_distance(c) <= s.td_radius)
			{
			return true;
			}
		}
	return false;
	}

void problem2()
	{
	const auto _fname =
		"d15p1.txt";
	//"d15p1_test.txt";
	auto sensors = day15_input(_fname);


	// Now iterate over each sensor and walk the border of its range. For each coordinate that borders the edge,
	// check if that coordinate is within the search area of another sensor

	for (const auto& s : sensors)
		{
		_debug(s.print());
		}

	std::vector<coord_t> found_positions;
	for (const auto& s : sensors)
		{
		// Walk the perimeter
		coord_t perimeter(s.loc.x, s.loc.y + s.td_radius + 1);	// Start at the bottom of the diamond
		// Start at the bottom of the diamond.
		// Check the coord underneath
		if (!check_other_sensors(sensors, perimeter))
			{
			found_positions.push_back(perimeter);
			}
		while (perimeter.y != s.loc.y)
			{
			if (!check_other_sensors(sensors, perimeter))
				{
				found_positions.push_back(perimeter);
				}
			perimeter.y--;
			perimeter.x++;
			}
		while (perimeter.x != s.loc.x)
			{
			if (!check_other_sensors(sensors, perimeter))
				{
				found_positions.push_back(perimeter);
				}
			perimeter.y--;
			perimeter.x--;
			}
		while (perimeter.y != s.loc.y)
			{
			if (!check_other_sensors(sensors, perimeter))
				{
				found_positions.push_back(perimeter);
				}
			perimeter.y++;
			perimeter.x--;
			}
		while (perimeter.x != s.loc.x)
			{
			if (!check_other_sensors(sensors, perimeter))
				{
				found_positions.push_back(perimeter);
				}
			perimeter.y++;
			perimeter.x++;
			}
		if (!check_other_sensors(sensors, perimeter))
			{
			found_positions.push_back(perimeter);
			}
		std::cout << found_positions.size() << std::endl;
		}
	}	// void problem2()
}	//namespace day_15
namespace day_16
	{
	static std::vector<std::string> _results;
	std::string print_route(const std::vector<std::string>& v)
		{
		std::string rv;
		for (const auto& s : v)
			{
			rv.append(s);
			rv.append("-");
			}
		return rv;
		}

	bool compare(const std::vector<std::string>& lhs, const std::vector<std::string>& rhs)
		{
		if (lhs.size() == rhs.size())
			{
			for (int i = 0; i < lhs.size(); ++i)
				{
				if (lhs[i] != rhs[i])
					{
					return false;
					}
				}
			return true;
			}
		else
			{
			return false;
			}
		}

	//static std::vector<size_t> all_path_scores;
	static size_t max_score = 0;


	struct valve_state_only_t
		{
		bool bIsOpen = false;
		size_t flow_rate;
		};

	using cavern_state_t = std::map<std::string, valve_state_only_t>;

	struct valve_t
		{
		using valves = std::vector<valve_t>;
		static valve_t& valve_from_name(const std::string& name, valve_t::valves& valves)
			{
			return *std::find_if(valves.begin(), valves.end(), [&name](auto& v) { return name == v.name; });
			}
		static const valve_t& valve_from_name(const std::string& name, const valve_t::valves& valves)
			{
			return *std::find_if(valves.begin(), valves.end(), [&name](auto& v) { return name == v.name; });
			}
		friend bool operator< (const valve_t& lhs, const valve_t& rhs)
			{
			return lhs.name < rhs.name;
			}

		valve_t(valve_t&) = default;
		valve_t(valve_t&&) = default;
		valve_t(std::string n, size_t f, std::vector<std::string>&& v) : name(n), flow_rate(f), dest_valves_names(v) {}

		const std::string name;
		size_t flow_rate = 0;
		bool bIsOpen = false;

		// Valves we can get to from this valve
		std::vector<std::string> dest_valves_names;

		// Map of valve names (valves with flow rates) and the shortest "distance" to that valve as a vector of valve names on the path.
		std::map<std::string, std::vector<std::string>> valves_of_interest;

		void navigate(std::vector<std::string> path_so_far, valve_t& source, const size_t max_depth, valve_t& target, valve_t::valves& all_valves)
			{
			//_debug(std::format("at {} seeking {} current depth {} of {}", name, target, path_so_far.size(), max_depth));
			if (path_so_far.size() >= max_depth)
				{
				//_debug("Too many steps");
				return;
				}
			path_so_far.push_back(name);

			// Exit conditions
			if (name == target.name)
				{
				//_debug(std::format("target found"));
				if ((path_so_far.size() < source.valves_of_interest[target.name].size()) || (source.valves_of_interest[target.name].size() == 0))
					{
					source.valves_of_interest[target.name] = path_so_far;
					}
				return;
				}

			for (const auto& t : dest_valves_names)
				{
				// Skip places we've already been
				if (std::find(path_so_far.begin(), path_so_far.end(), t) != path_so_far.end())
					{
					continue;
					}

				valve_from_name(t, all_valves).navigate(path_so_far, source, max_depth, target, all_valves);
				}
			return;
			}

		struct move_t
			{
			move_t() = default;
			move_t(size_t d, const valve_t* t, size_t s) : dist(d), target(t), score(s) {}
			size_t dist = 0;
			const valve_t* target = nullptr;
			size_t score = 0;
			// This is a terrible hack to implement the sorting order we want
			static bool sort (const move_t& lhs, const move_t& rhs) { return lhs.score > rhs.score; }
			};

		using moves_t = std::vector<move_t>;

		void play(std::vector<std::string> path_so_far, const valves& valves, size_t minute, size_t score, cavern_state_t state/*, std::string this_path = std::string() */) const
			{
			if (minute > 30)
				{
				return;
				}
			//this_path.append(std::format("arrived {} score {}, minute {}\r\n", name, score, minute));
			for (auto& v : state)
				{
				if (v.second.bIsOpen)
					{
					score += v.second.flow_rate;
					}
				}

			//this_path.append(std::format("score increased to {}\r\n", score));

			// Spend 1 minute here opening the valve
			path_so_far.push_back(name);
			minute++;
			state[name].bIsOpen = true;

			bool bAllOn = true;
			for (auto& v : state)
				{
				if (!v.second.bIsOpen)
					{
					bAllOn = false;
					break;
					}
				}

			// Look for the next move
			bool bHaveMoves = false;
			if (!bAllOn)
				{
				moves_t moves;
				
				for (auto& v : state)
					{
					if (v.first == name)	// skip self
						{
						continue;
						}
					if (!v.second.bIsOpen)
						{
						size_t dist = valve_from_name(name, valves).valves_of_interest.at(v.first).size();
						if (dist < (30 - minute))	// No bother if we're going to run out of time en route
							{
							const valve_t* tgt = &valve_from_name(v.first, valves);
							int score = (int(v.second.flow_rate) - int(dist) + 1) * (30 - minute - 1 - int(dist));
							if (score < 0) { score = 0; }
							move_t m(dist, tgt, score);
							moves.push_back(m);
							}
						}
					}

				// Cull the bottom moves
				if (moves.size())
					{
					bHaveMoves = true;
					//std::sort(moves.begin(), moves.end(), &move_t::sort);

					// With Ian's day 16 input, in a Release build, a Cull size of 3 takes < 30 seconds.
					//constexpr size_t _cullsize = 7;
					//auto culled = moves_t(moves.begin(), /*moves.end());*/moves.begin() + std::min(size_t(_cullsize), moves.size()));
					//moves = std::move(culled);
					}

				/*
				if (culled.size() == 0)
					{
					throw (-9);
					}
					*/
				/*
				if (compare(path_so_far, std::vector<std::string>  { "AA", "DD"})
					|| compare(path_so_far, std::vector<std::string> { "AA", "DD", "BB"})
					|| compare(path_so_far, std::vector<std::string> { "AA", "DD", "BB", "JJ"})
					|| compare(path_so_far, std::vector<std::string> { "AA", "DD", "BB", "JJ", "HH"})
					|| compare(path_so_far, std::vector<std::string> { "AA", "DD", "BB", "JJ", "HH", "EE"})
					|| compare(path_so_far, std::vector<std::string> { "AA", "DD", "BB", "JJ", "HH", "EE", "CC"}))
					{
					std::cout << "";
					}
					*/

				for (auto m : moves)
					{
					size_t minutes_to_target = m.dist - 1;
					// Play out the "journey"
					size_t score_for_this_target = 0;
					for (auto& v : state)
						{
						if (v.second.bIsOpen)
							{
							score_for_this_target += v.second.flow_rate * minutes_to_target;
							}
						}
					m.target->play(path_so_far, valves, (minute + minutes_to_target), score + score_for_this_target/*, this_path */, state);
					}
				}
			// Otherwise just play out the rest of the game
			if (bAllOn || !bHaveMoves)
				{
				while (minute <= 30)
					{
					++minute;
					for (auto& v : state)
						{
						if (v.second.bIsOpen)
							{
							score += v.second.flow_rate;
							}
						}
					}
				//_debug(this_path);
				_results.push_back(std::format("Score {} for path {}", score, print_route(path_so_far)));
				//all_path_scores.push_back(score);
				if (max_score == 0 || (max_score < score))
					{
					max_score = score;
					}
				return;
				}
			}
		};


	valve_t::valves day16_input(const std::string& _fname)
		{
		auto lines = string_vec_input(_fname);

		valve_t::valves rv;
		for (const auto& line : lines)
			{
			std::string name = std::string(line, 6, 2);
			size_t fr = std::stoi(std::string(line, line.find_first_of("-0123456789"), line.length()));
			auto h2_start = line.find("; t");
			auto _seek = line.contains("valves") ? "valves" : "valve";
			auto _dist = line.contains("valves") ? 7 : 6;
			auto valves_start = line.find(_seek, h2_start) + _dist;

			auto valves = _get_delimited_vals(std::string(line, valves_start, line.length()), ',', true);

			rv.push_back(valve_t(name, fr, std::move(valves)));
			}
		return rv;
		}

	// Returns the score. Don't go all the way to the end of time, just 1 move after the last valve is opened.
	size_t score_from_move(const std::vector<std::string>& path, valve_t::valves& valves, size_t minutes_remaining, const std::string& start)
		{
		size_t rv = 0;
		size_t score_per_minute = 0;
		for (const auto v : path)
			{
			if (minutes_remaining == 0)
				{
				break;
				}

			rv += score_per_minute;
			if (v == start)
				{
				continue;
				}
			auto& valve = valve_t::valve_from_name(v, valves);
			if (!valve.bIsOpen)
				{
				score_per_minute += valve.flow_rate;
				}
			minutes_remaining--;
			}


		rv += score_per_minute;
		return rv;
		}

	void problem1()
		{
		const std::string _fname =
			"d16p1.txt";
			//"d16p1_test.txt";
		auto valves = day16_input(_fname);

		// Populate "routes" from each tunnel to each destination tunnel.
		std::vector<std::reference_wrapper<valve_t>> valves_with_values;
		for (auto& v : valves)
			{
			// Populate the list of valves that have values
			if (v.flow_rate)
				{
				valves_with_values.push_back(v);
				}
			}

		// For each valve (we might want to visit), populate paths to other valves that have flow rates (we may want to visit)
		for (auto& v : valves)
			{
			for (auto& tv : valves_with_values)
				{
				v.valves_of_interest.insert(std::make_pair(tv.get().name, std::vector<std::string>()));
				}
			}

		// Populate all of the paths
		for (auto& outer : valves)
			{
			//_debug("******************************");
			for (auto& v : outer.valves_of_interest)
				{
				std::vector<std::string> path_so_far;
				//_debug(std::format("** at {} seeking shortest to {}", outer.name, v.first.get().name));
				if (outer.name == v.first)
					{
					// Path to self unnecessary
					continue;
					}
				else
					{
					outer.navigate(path_so_far, outer, valves.size(), valve_t::valve_from_name(v.first, valves), valves);
					}
				//_debug(std::format("valve {} to {} shortest path {}", outer.name, v.first, print_route(v.second)));
				}

			}

		// Now that we have the path from each valve to each other valve, do some calculating.
		size_t minute = 0;
		size_t current_score = 0;

		cavern_state_t state;

		for (auto& v : valves[0].valves_of_interest)
			{
			valve_state_only_t t;
			const auto& valve = valve_t::valve_from_name(v.first, valves);
			t.bIsOpen = false;
			t.flow_rate = valve.flow_rate;
			state.insert(std::make_pair(v.first, t));
			}

		
		valve_t::valve_from_name("AA", valves).play(std::vector<std::string>(), valves, minute, 0, state);
		
		//std::sort(all_path_scores.begin(), all_path_scores.end(), std::greater<>());
		std::cout << "max score: " << max_score << std::endl;
 		}

	struct player_t
		{
		player_t(const std::string& loc) : current_target_valve(loc) {}
		std::string current_target_valve;
		size_t minutes_remaining_to_target = 0;
		};

	struct cavern_state_p2_t
		{
		cavern_state_p2_t(const std::string& h, const std::string& e) : elephant(player_t(e)), human(player_t(h)) {}
		player_t elephant;
		player_t human;
		std::map<std::string, valve_state_only_t> cavern;
		};

	const size_t __minutes = 26;

	void p2play(cavern_state_p2_t c, size_t minute, size_t score, std::vector<std::string> path_so_far, const valve_t::valves& valves)
		{
		//_debug(std::format("minute {}, h={},{} | e={},{}", minute, c.human.current_target_valve, c.human.minutes_remaining_to_target, c.elephant.current_target_valve, c.elephant.minutes_remaining_to_target));
		if (minute > __minutes)
			{
			return;
			}
		++minute;
		//this_path.append(std::format("arrived {} score {}, minute {}\r\n", name, score, minute));
		for (auto& v : c.cavern)
			{
			if (v.second.bIsOpen)
				{
				score += v.second.flow_rate;
				}
			}

		if (minute < __minutes)
			{
			auto next_paths = [&c, &valves, &minute](const std::string& cur_loc, const std::string& other_loc) -> valve_t::moves_t
				{
				valve_t::moves_t moves;

				for (auto& v : c.cavern)
					{
					if (v.first == cur_loc || v.first == other_loc)	// skip current location and the destination of the other player
						{
						continue;
						}
					if (!v.second.bIsOpen)
						{
						size_t dist = valve_t::valve_from_name(cur_loc, valves).valves_of_interest.at(v.first).size();
						if (dist < (__minutes - minute))	// No bother if we're going to run out of time en route
							{
							const valve_t* tgt = &valve_t::valve_from_name(v.first, valves);
							int score = (int(v.second.flow_rate) - int(dist) + 1) * (__minutes - minute - 1 - int(dist));
							if (score < 0) { score = 0; }
							valve_t::move_t m(dist, tgt, score);
							moves.push_back(m);
							}
						}
					}
				return moves;
				};

			std::queue<valve_t::move_t> human_moves;
			std::queue<valve_t::move_t> elephant_moves;

			if (c.human.minutes_remaining_to_target)
				{
				c.human.minutes_remaining_to_target--;
				}
			// open the valve and Pick a new target for H
			else
				{
				path_so_far.push_back(std::format("h:{}", c.human.current_target_valve));
				c.cavern.at(c.human.current_target_valve).bIsOpen = true;
				// Spend 1 minute here opening the valve and planning what to do next.
				auto h_moves = next_paths(c.human.current_target_valve, c.elephant.current_target_valve);
				std::sort(h_moves.begin(), h_moves.end(), &valve_t::move_t::sort);
				for (const auto& m : h_moves)
					{
					human_moves.push(m);
					}
				}

			if (c.elephant.minutes_remaining_to_target)
				{
				c.elephant.minutes_remaining_to_target--;
				}
			// Pick a new target for E
			else
				{
				path_so_far.push_back(std::format("e:{}", c.elephant.current_target_valve));
				c.cavern.at(c.elephant.current_target_valve).bIsOpen = true;
				// Spend 1 minute here opening the valve and planning what to do next.
				auto e_moves = next_paths(c.elephant.current_target_valve, c.human.current_target_valve);
				std::sort(e_moves.begin(), e_moves.end(), &valve_t::move_t::sort);
				for (const auto& m : e_moves)
					{
					elephant_moves.push(m);
					}
				}
			if (human_moves.size() || elephant_moves.size())
				{
				while (human_moves.size() || elephant_moves.size())
					{
					if (!human_moves.empty())
						{
						auto& m = human_moves.front();
						size_t minutes_to_target = m.dist - 1;
						c.human.current_target_valve = m.target->name;
						c.human.minutes_remaining_to_target = minutes_to_target;
						human_moves.pop();
						}
					if (!elephant_moves.empty())
						{
						auto m = elephant_moves.front();
						// Because we're choosing elephant second, we need to make sure they don't go to the same location as the person
						while (elephant_moves.size() && m.target->name == c.human.current_target_valve)
							{
							elephant_moves.pop();
							if (elephant_moves.size())
								{
								m = elephant_moves.front();
								}
							}
						if (elephant_moves.size())
							{
							size_t minutes_to_target = m.dist - 1;
							c.elephant.current_target_valve = m.target->name;
							c.elephant.minutes_remaining_to_target = minutes_to_target;
							elephant_moves.pop();
							}
						}
					p2play(c, minute, score, path_so_far, valves);
					}
				}
			else
				{
				p2play(c, minute, score, path_so_far, valves);
				}
			}

		// If we get here both the elephand and human are at a destination but cannot reach another destination before time expires.
		else
			{
			if ((max_score == 0 || (max_score < score)))
				{
				_debug(std::format("{} score for path {}", score, print_route(path_so_far)));
				max_score = score;
				}
			}
		}

	void problem2()
		{
		const std::string _fname =
			"d16p1.txt";
		//"d16p1_test.txt";
		auto valves = day16_input(_fname);

		// Populate "routes" from each tunnel to each destination tunnel.
		std::vector<std::reference_wrapper<valve_t>> valves_with_values;
		for (auto& v : valves)
			{
			// Populate the list of valves that have values
			if (v.flow_rate)
				{
				valves_with_values.push_back(v);
				}
			}

		// For each valve (we might want to visit), populate paths to other valves that have flow rates (we may want to visit)
		for (auto& v : valves)
			{
			for (auto& tv : valves_with_values)
				{
				v.valves_of_interest.insert(std::make_pair(tv.get().name, std::vector<std::string>()));
				}
			}

		// Populate all of the paths
		for (auto& outer : valves)
			{
			//_debug("******************************");
			for (auto& v : outer.valves_of_interest)
				{
				std::vector<std::string> path_so_far;
				//_debug(std::format("** at {} seeking shortest to {}", outer.name, v.first.get().name));
				if (outer.name == v.first)
					{
					// Path to self unnecessary
					continue;
					}
				else
					{
					outer.navigate(path_so_far, outer, valves.size(), valve_t::valve_from_name(v.first, valves), valves);
					}
				//_debug(std::format("valve {} to {} shortest path {}", outer.name, v.first, print_route(v.second)));
				}

			}

		// Now that we have the path from each valve to each other valve, do some calculating.
		size_t minute = 0;
		size_t current_score = 0;

		cavern_state_p2_t c("AA", "AA");

		for (auto& v : valves[0].valves_of_interest)
			{
			valve_state_only_t t;
			const auto& valve = valve_t::valve_from_name(v.first, valves);
			t.bIsOpen = false;
			t.flow_rate = valve.flow_rate;
			c.cavern.insert(std::make_pair(v.first, t));
			}

		// Populate AA
		valve_state_only_t t;
		t.bIsOpen = true;
		t.flow_rate = 0;
		c.cavern.insert(std::make_pair("AA", t));


		p2play(c, minute, 0, std::vector<std::string>(), valves);

		//std::sort(all_path_scores.begin(), all_path_scores.end(), std::greater<>());
		std::cout << "max score: " << max_score << std::endl;
		}

	}	// namespace day_16

namespace day_21
{
struct monkey_t
	{
	monkey_t() = default;
	// Just number ctor
	monkey_t(const std::string& n, int v) : value(v), bHasValue(true), name (n) {}
	// Other Monkey ctor
	monkey_t(const std::string& n, char op, const std::string& m1, const std::string& m2) : name(n), monkey1 (m1), monkey2 (m2), operation (op) {}
	bool bHasValue = false;
	int64_t value = 0;

	const std::string name;
	const std::string monkey1;
	const std::string monkey2;
	char operation = ' ';

	void action(int64_t lv, int64_t rv)
		{
		switch (operation)
			{
			case '+':
				bHasValue = true;
				value = lv + rv;
				break;

			case '-':
				bHasValue = true;
				value = lv - rv;
				break;

			case '*':
				bHasValue = true;
				value = lv * rv;
				break;
			case '/':
				bHasValue = true;
				value = lv / rv;
				break;

			default:
				throw - 7;
				break;
			
			}

		}
	};
using monkeys = std::vector<monkey_t>;

monkeys day21_input()
	{
	monkeys rv;
	auto _fname =
		"d21p1.txt";
		//"d21p1_test.txt";

	auto input = string_vec_input(_fname);

	for (const auto& l : input)
		{
		std::string name = std::string(l.begin(), l.begin() + 4);
		if (isdigit(l.at(6)))
			{
			// Monkey with just a number
			auto nums = _get_all_numbers(l);
			if (nums.size() != 1)
				{
				throw (-6);
				}

			rv.push_back(monkey_t(name, nums[0]));
			}
		else
			{
			// Monkey that gets its input from other monkeys
			// First monkeys name indicies 6-9 incl, Second Mokney 13-16 incl
			std::string m1 = std::string(l.begin() + 6, l.begin() + 10);
			std::string m2 = std::string(l.begin() + 13, l.end());

			//operation is at position 11

			rv.push_back(monkey_t(name, l.at(11), m1, m2));
			}
		}
	return rv;
	}

	void problem1()
		{
		auto all_monkeys = day21_input();


		std::map<std::string, monkey_t> easy_search;

		for (const auto& m : all_monkeys)
			{
			easy_search.insert(std::make_pair(m.name, m));
			}


		bool bSomethingChanged = true;
		while (bSomethingChanged)
			{
			bSomethingChanged = false;
			for (auto& m : easy_search)
				{
				if (!m.second.bHasValue)
					{
					// See if the other monkeys we depend on have values
					if (easy_search[m.second.monkey1].bHasValue && easy_search[m.second.monkey2].bHasValue)
						{
						bSomethingChanged = true;
						m.second.action(easy_search[m.second.monkey1].value, easy_search[m.second.monkey2].value);
						_debug(std::format("Computing value for monkey {} op {} result {} from {} and {}", m.first, m.second.operation, m.second.value, easy_search[m.second.monkey1].value, easy_search[m.second.monkey2].value));
						}
					}
				}
			}
		}
}	//namespace day_21
int main()
	{
	day_21::problem1();
  	}
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

	// Represents the current location and any possible future moves from this location.
	struct move_t
		{
		enum move
			{
			up = '^',
			down = 'v',
			left = '<',
			right = '>'
			};

		move_t(int x, int y, move _m, move_t* p)
			: m(_m)
			, cur_loc(std::make_pair(x, y))
			, parent(p)
			{ }

		move m = up;

		grid_t::_coord cur_loc = std::make_pair(0, 0);

		move_t* parent = nullptr;
		std::vector<move_t> next;

		bool is_valid_move(const grid_t& g, int x, int y)
			{
			//std::cout << "d12_debug: checking move to " << _print(x, y) << std::endl;
			// Grid boundaries check
			if (x >= g.grid.size() || y >= g.grid[0].size() || x < 0 || y < 0)
				{
				//std::cout << "d12_debug: rejecting move from " << _print(cur_loc) << " to " << _print(x, y) << " due to grid boundary" << std::endl;
				return false;
				}

			// Not a valid move to pass through a location we've already passed through.
			move_t* p = parent;
			while (p != nullptr)
				{
				if (p->cur_loc == std::make_pair(x, y))
					{
					//std::cout << "d12_debug: rejecting move from " << _print(cur_loc) << " to " << _print(x, y) << " due to repeated location" << std::endl;
					return false;
					}
				p = p->parent;
				}
			// Can only increment by max of one letter.
			if (g.get(x, y) - g.get(cur_loc) > 1)
				{
				//std::cout << "d12_debug: rejecting move from " << _print(cur_loc) << " to " << _print(x, y) << " due to height difference" << std::endl;
				return false;
				}

			// Don't bother going down more tha one level either
			if (g.get(cur_loc) - g.get(x, y) > 1)
				{
				return false;
				}

			return true;
			}

		bool is_end(const grid_t& g, int x, int y) const
			{
			return g.end.first == x && g.end.second == y;
			}

		bool should_prune() const
			{
			// Solved by hand with <450 moves;
			// Should hit column 45 within 75 moves.
			// Should hit column 17 in 17 moves
			size_t parents = count_parents();

			if (parents > 450)
				{
				return true;
				}
			if ((parents > 17 && cur_loc.first < 17))
				{
				return true;
				}
			if ((parents > 75 && cur_loc.first < 45))
				{
				return true;
				}

			// once we're past col 57 we should never need to go below it.
			// Before we get to column 57 we should never have to go backward
			// We should also never need to be y < 7 or y > 37
			if (cur_loc.second < 7 || cur_loc.second > 37)
				{
				return true;
				}

			if (parent->cur_loc.first >= 57 && cur_loc.first < 57)
				{
				return true;
				}
			else if (parent->cur_loc.first < 57 && cur_loc.first < parent->cur_loc.first)
				{
				return true;
				}

			return false;
			}

		grid_t::_coord calc_move(const move_t::move& m) const
			{
			const std::map<const move_t::move, const grid_t::_coord> _map =
				{
						 {move::up,{0,-1}}
						,{move::down,{0,1}}
						,{move::left,{-1,0}}
						,{move::right,{1,0}}
				};

			return _map.at(m);
			}

		void explore_moves(const grid_t& g)
			{
			_for_display.grid[cur_loc.first][cur_loc.second] = 'X';
			_cycles++;
			if (_cycles % 1000000 == 0)
				{
				system("cls");
				_for_display.print();
				}

			//std::cout << "d12_debug: exploring from " << _print(cur_loc) << std::endl;
			// Explore in UDLR
			std::vector<std::pair<move_t::move, grid_t::_coord>> moves =
				{
					 {move::up,{cur_loc.first,cur_loc.second - 1}}
					,{move::down,{cur_loc.first,cur_loc.second + 1}}
					,{move::left, {cur_loc.first - 1, cur_loc.second}}
					,{move::right, {cur_loc.first + 1,cur_loc.second}}
				};

			for (const auto& m : moves)
				{
				if (is_valid_move(g, m.second.first, m.second.second))
					{
					// Still add the end so we can check if we made it later
					next.push_back(move_t(m.second.first, m.second.second, m.first, this));
					if (is_end(g, m.second.first, m.second.second))	// Exit condition
						{
						return;
						}
					else if (should_prune())
						{
						// Prune large sections of this tree if we haven't found the end after a reasonable amount of time.
						return;
						}

					}
				}

			// If we didn't reach the end, keep going

			for (auto& m : next)
				{
				m.explore_moves(g);
				}
			}

		size_t count_parents() const
			{
			// Count the steps (parents) back to root
			size_t steps(0);
			move_t* p = parent;
			while (p != nullptr)
				{
				steps++;
				p = p->parent;
				}
			return steps;
			}

		void count_moves(size_t& min, const grid_t& g) const
			{
			for (const auto& m : next)
				{
				if (is_end(g, m.cur_loc.first, m.cur_loc.second))
					{
					size_t steps = count_parents();
					if (steps < min)
						{
						min = steps;
						}
					return;
					}
				else
					{
					m.count_moves(min, g);
					}
				}
			return;
			}
		};

	void problem1()
		{
		auto grid = day12_input();

		move_t root(grid.start.first, grid.start.second, move_t::move::down, nullptr);
		root.cur_loc = grid.start;

		// Hack: Start by moving 19 right and 12 down, as we know we'll need to pass through that 'c'

		std::vector <move_t::move> initial_moves =
			{
			move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right,
			move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right, move_t::right,
			move_t::down, move_t::down, move_t::down, move_t::down, move_t::down, move_t::down, move_t::down, move_t::down, move_t::down, move_t::down,
			move_t::down, move_t::down
			};

		move_t* current = &root;
		for (auto& move : initial_moves)
			{
			grid_t::_coord c = current->cur_loc;
			grid_t::_coord calc = current->calc_move(move);
			c.first += calc.first;
			c.second += calc.second;

			// Add the move. We're assuming because I did it by hand that it's valid.
			current->next.push_back(move_t(c.first, c.second, move, current));
			current = &(current->next[0]);
			}

		_for_display = grid;

		// This is going to be very memory intensive
		current->explore_moves(grid);

		size_t min_moves = 9999999999;

		root.count_moves(min_moves, grid);

		std::cout << "min moves=" << min_moves << std::endl;
		}

	}
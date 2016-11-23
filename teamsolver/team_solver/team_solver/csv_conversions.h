#pragma once

#include "csv.h"
#include "player.h"

player_position string_to_pos (std::string pos)
	{
	if (pos == "QB")
		{
		return player_position::qb;
		}
	if (pos == "RB")
		{
		return player_position::rb;
		}
	if (pos == "WR")
		{
		return player_position::wr;
		}
	if (pos == "TE")
		{
		return player_position::te;
		}
	if (pos == "DST")
		{
		return player_position::dst;
		}
	// If we get here, something bad happened.
	throw std::exception ();
	}


// The line from the CSV file is formatted as follows:
// "Position", "Name", "Salary", "GameInfo", "AvgPointsPerGame", "teamAbbrev"
enum class dk_data_loc
	{
	position = 0,
	name = 1,
	salary = 2,
	game_info = 3,
	appg = 4,
	team = 5,
	col_count = 6,
	};

enum class rg_data_loc
	{
	name = 0,
	salary = 1,
	team = 2,
	position = 3,
	opp_team = 4,
	ceiling = 5,
	floor = 6,
	project_pts = 7,
	col_count = 8,
	};

inline dk_player_data_t dk_csv_str_to_player_data (const csv::line_t& player)
	{
	if (player.size () != static_cast<size_t>(dk_data_loc::col_count))
		{
		throw std::exception ();
		}

	dk_player_data_t rv;
	rv.name = player[static_cast<size_t>(dk_data_loc::name)];
	rv.position = string_to_pos (player[static_cast<size_t>(dk_data_loc::position)]);
	rv.salary = atoi (player[static_cast<size_t>(dk_data_loc::salary)].c_str ());
	rv.appg = atof (player[static_cast<size_t>(dk_data_loc::appg)].c_str ());

	return rv;
	}



inline rg_player_data_t rg_csv_str_to_player_data (const csv::line_t& player)
	{
	if (player.size () != static_cast<size_t>(rg_data_loc::col_count))
		{
		throw std::exception ();
		}

	rg_player_data_t rv;
	rv.name = player[static_cast<size_t>(rg_data_loc::name)];
	rv.position = string_to_pos (player[static_cast<size_t>(rg_data_loc::position)]);
	rv.team = player[static_cast<size_t>(rg_data_loc::team)];
	rv.opp_team = player[static_cast<size_t>(rg_data_loc::opp_team)];
	rv.salary = atoi (player[static_cast<size_t>(rg_data_loc::salary)].c_str ());
	rv.ceiling = atof (player[static_cast<size_t>(rg_data_loc::ceiling)].c_str ());
	rv.floor = atof (player[static_cast<size_t>(rg_data_loc::floor)].c_str ());
	rv.projected_pts = atof (player[static_cast<size_t>(rg_data_loc::project_pts)].c_str ());

	return rv;
	}

// Copyright 2016 Ian Maines
#include "csv.h"
#include "player.h"
#include "team.h"
#include "csv_conversions.h"

#include <map>
#include <queue>
#include <iostream>
#include <algorithm>

namespace
{
void playermap_init (std::map<player_position, std::vector<player>>& map)
	{
	if (map.size ())
		{
		throw std::exception ();
		}
	map.insert (std::make_pair (player_position::qb, std::vector<player> ()));
	map.insert (std::make_pair (player_position::rb, std::vector<player> ()));
	map.insert (std::make_pair (player_position::wr, std::vector<player> ()));
	map.insert (std::make_pair (player_position::te, std::vector<player> ()));
	map.insert (std::make_pair (player_position::flex, std::vector<player> ()));
	map.insert (std::make_pair (player_position::dst, std::vector<player> ()));
	}

bool threshold (const player& p)
	{
	switch (p.GetPosition ())
		{
		case player_position::qb:
			return p.GetAPPG () > 15 ||
				p.GetProjectedPoints () > 17;

			break;

		case player_position::rb:
			return p.GetAPPG () > 14 ||
				p.GetProjectedPoints () > 16;
			break;
		case player_position::wr:
			return p.GetAPPG () > 12 ||
				p.GetProjectedPoints () > 14;
			break;

		case player_position::te:
			return p.GetAPPG () > 10 ||
				p.GetProjectedPoints () > 12;
			break;

		case player_position::dst:
			return p.GetAPPG () > 8 ||
				p.GetProjectedPoints () > 10;
			break;
		default:
			throw std::exception ();
			break;
		}
	return false;
	}
}

int main ()
	{
// *******************************************************************
// Grab draft kings data.
// *******************************************************************
	std::map <std::string, dk_player_data_t> dk_player_map;
	{
	csv::CDkCsv DKcsv ("draft_kings.csv");
	if (!DKcsv.Import ())
		{
		return -1;
		}

	csv::lines_t dk_lines = DKcsv.GetData ();

	// Get rid of the title line.
	dk_lines.erase (dk_lines.begin (), dk_lines.begin () + 1);

	for (const auto& line : dk_lines)
		{
		dk_player_data_t p (dk_csv_str_to_player_data (line));
#ifdef _DEBUG
		if (dk_player_map.find (p.name) != dk_player_map.end ())
			{
			std::cout << "Duplicate player found in RG list: " << p.name << "\n";
			}
#endif
		dk_player_map.insert (std::make_pair (p.name, p));
		}
	}

// *******************************************************************
// Grab RotoGrinder data.
// *******************************************************************
	std::map <std::string, rg_player_data_t> rg_player_map;
	{
	csv::CRgCsv RGcsv ("rotogrinder.csv");
	if (!RGcsv.Import ())
		{
		return -1;
		}

	csv::lines_t rg_lines = RGcsv.GetData ();

	// Get rid of the title line. It's at the end...?
	rg_lines.erase (rg_lines.end () - 1, rg_lines.end ());	// bug if empty vector

	for (const auto& line : rg_lines)
		{
		rg_player_data_t p (rg_csv_str_to_player_data (line));
#ifdef _DEBUG
		if (rg_player_map.find (p.name) != rg_player_map.end ())
			{
			std::cout << "Duplicate player found in RG list: " << p.name << "\n";
			}
#endif
		rg_player_map.insert (std::make_pair (p.name, p));
		}
	}

// *******************************************************************
// Build up the player map.
// *******************************************************************
	std::map<player_position, std::vector<player>> all_players;
	playermap_init (all_players);

	// Iterate all of the dk players - rotogrinder includes players on bye
	for (const auto& dk_p_data : dk_player_map)
		{
		auto itr = rg_player_map.find (dk_p_data.second.name);
		if (itr != rg_player_map.end ())
			{
			player p (dk_p_data.second, itr->second);
			if (threshold (p))
				{
				all_players[p.GetPosition ()].push_back (p);
				if (is_flex (p.GetPosition ()))
					{
					all_players[player_position::flex].push_back (p);
					}
				}
			}
		else
			{
			std::cout << "DraftKings player not found in RG list: " << dk_p_data.second.name << "\n";
			}
		}

	// Now the fun part... Start building teams.
	std::priority_queue<team> top_teams;
	double combos (707888160000/*all_players[player_position::qb].size () *
				   all_players[player_position::rb].size () *
				   all_players[player_position::rb].size () *
				   all_players[player_position::wr].size () *
				   all_players[player_position::wr].size () *
				   all_players[player_position::wr].size () *
				   all_players[player_position::te].size () *
				   all_players[player_position::flex].size () *
				   all_players[player_position::dst].size ()*/);

	size_t teams_found (0);

	std::cout << "Combos: " << combos << "\n";
	double iterations (0);
	bool bBreak (false);
	for (auto& qb : all_players[player_position::qb])
		{
		for (auto& rb1 : all_players[player_position::rb])
			{
			for (auto& rb2 : all_players[player_position::rb])
				{
				for (auto& wr1 : all_players[player_position::wr])
					{
					for (auto& wr2 : all_players[player_position::wr])
						{
						for (auto& wr3 : all_players[player_position::wr])
							{
							for (auto& te : all_players[player_position::te])
								{
								for (auto& flex : all_players[player_position::flex])
									{
									for (auto& dst : all_players[player_position::dst])
										{
										iterations++;
										if (static_cast<long long unsigned int>(iterations) % 10000000 == 0)
											{
											std::cout << "Iteration '" << iterations << "', Progress = '" << iterations / combos * 100 << "%' Top teams size: '" << top_teams.size () << "' Teams Found: '" << teams_found <<"'\n";
											}
										try
											{
											if (is_team_valid (qb, rb1, rb2, wr1, wr2, wr3, te, flex, dst))
												{
												teams_found++;
												team t (qb, rb1, rb2, wr1, wr2, wr3, te, flex, dst);
												if (top_teams.size () < 100)
													{
													top_teams.push (t);
													//std::sort (top_teams.begin (), top_teams.end (), [](const team& t1, const team& t2) { return t1.GetTotalAPPG () < t2.GetTotalAPPG (); });
													}
												else if (t.total_appg > (top_teams.top ()).total_appg)
													{
													top_teams.pop ();
													top_teams.push (t);
													//std::sort (top_teams.begin (), top_teams.end (), [](const team& t1, const team& t2) { return t1.GetTotalAPPG () < t2.GetTotalAPPG (); });
													}
												}
											}
										catch (...)
											{ }
										}
									}
								}
							}
						}
					}
				}
			}
		}

	while (top_teams.size ())
		{
		team t = top_teams.top ();
		top_teams.pop ();
		std::cout << "\n\nTeam: \n" << t.total_appg << ", " << t.total_salary <<
			"QB  : " << t.m_qb.GetName () << ", " << t.m_qb.GetAPPG () << "\n" <<
			"RB1 : " << t.m_rb1.GetName () << ", " << t.m_rb1.GetAPPG () << "\n" <<
			"RB2 : " << t.m_rb2.GetName () << ", " << t.m_rb2.GetAPPG () << "\n" <<
			"WR1 : " << t.m_wr1.GetName () << ", " << t.m_wr1.GetAPPG () << "\n" <<
			"WR2 : " << t.m_wr2.GetName () << ", " << t.m_wr2.GetAPPG () << "\n" <<
			"WR3 : " << t.m_wr3.GetName () << ", " << t.m_wr3.GetAPPG () << "\n" <<
			"TE  : " << t.m_te.GetName () << ", " << t.m_te.GetAPPG () << "\n" <<
			"FLEX: " << t.m_flex.GetName () << ", " << t.m_flex.GetAPPG () << "\n" <<
			"DST : " << t.m_dst.GetName () << ", " << t.m_dst.GetAPPG () << "\n";
		}

    return 0;
	}


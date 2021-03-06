// Copyright 2016 Ian Maines
#include "csv.h"
#include "player.h"
#include "team.h"
#include "csv_conversions.h"

#include <map>
#include <queue>
#include <memory>
#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <chrono>
#include <sstream>
#include <fstream>
#include <ctime>


using player_map_t = std::map<player_position, std::vector<player>>;

namespace
{
void playermap_init (player_map_t& map)
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
				return p.GetProjectedPoints () > 15;
			break;

		case player_position::rb:
				return p.GetProjectedPoints () > 12;
			break;

		case player_position::wr:
				return p.GetProjectedPoints () > 11;
			break;

		case player_position::te:
				return p.GetProjectedPoints () > 9;
			break;

		case player_position::dst:
				return p.GetProjectedPoints () > 7;
			break;
		default:
			throw std::exception ();
			break;
		}
	return false;
	}




std::unique_ptr<team> worker (player& qb, player_map_t& all_players)
	{
	size_t cflex1 (0);
	size_t teams_found (0);
	std::unique_ptr<team> top_team (nullptr);
	for (auto& flex : all_players[player_position::flex])
		{
		cflex1++;
		std::cout << "FLEX updated. QB = " << qb.GetName () << ", FLEX = " << cflex1 << "\n";
		for (auto& rb1 : all_players[player_position::rb])
			{
			// Save ourselves some cycles.
			if (rb1.GetHash () == flex.GetHash ())
				{
				continue;
				}
			for (auto& rb2 : all_players[player_position::rb])
				{
				// Save ourselves some cycles.
				if (rb2.GetHash () == rb1.GetHash () || rb2.GetHash () == flex.GetHash ())
					{
					continue;
					}
				for (auto& wr1 : all_players[player_position::wr])
					{
					// save ourselves some cycles
					if (wr1.GetHash () == flex.GetHash ())
						{
						continue;
						}
					for (auto& wr2 : all_players[player_position::wr])
						{
						// save ourselves some cycles
						if (wr2.GetHash () == wr1.GetHash () || wr2.GetHash () == flex.GetHash ())
							{
							continue;
							}
						for (auto& wr3 : all_players[player_position::wr])
							{
							// save ourselves some cycles
							if (wr3.GetHash () == wr2.GetHash () || wr3.GetHash () == wr1.GetHash () || wr3.GetHash () == flex.GetHash ())
								{
								continue;
								}
							for (auto& te : all_players[player_position::te])
								{
								// save ourselves some cycles
								if (te.GetHash () == flex.GetHash ())
									{
									continue;
									}
								for (auto& dst : all_players[player_position::dst])
									{
									if (is_team_valid (qb, rb1, rb2, wr1, wr2, wr3, te, flex, dst))
										{
										teams_found++;
										team t (qb, rb1, rb2, wr1, wr2, wr3, te, flex, dst);
										if (!top_team)
											{
											top_team = std::make_unique<team> (t);
											}
										else if (t.total_projected_points > top_team->total_projected_points)
											{
											top_team = std::make_unique<team> (t);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	std::cout << "Found " << teams_found << " teams." << "projected score: " << top_team->total_projected_points << "\n";
	return top_team;
	}
}

int main ()
	{
			{
			time_t t = time (0);   // get time now
			struct tm now;
			localtime_s (&now, &t);
			std::cout << std::endl << (now.tm_hour) << ':'
				<< (now.tm_min) << ':'
				<< now.tm_sec
				<< std::endl;
			}

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
			std::cout << "Duplicate player found in DK list: " << p.name << "\n";
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
		if (rg_player_map.find (p.name) != rg_player_map.end ())
			{
			std::cout << "Duplicate player found in RG list: " << p.name << "\n";
			}
		rg_player_map.insert (std::make_pair (p.name, p));
		}
	}

// *******************************************************************
// Build up the player map.
// *******************************************************************
	player_map_t all_players;
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

	std::cout << "Player counts:\nQB: " << all_players[player_position::qb].size () << "\n"
		<< "Player counts:\nRB  : " << all_players[player_position::rb].size () << "\n"
		<< "Player counts:\nWR  : " << all_players[player_position::wr].size () << "\n"
		<< "Player counts:\nTE  : " << all_players[player_position::te].size () << "\n"
		<< "Player counts:\nFLEX: " << all_players[player_position::flex].size () << "\n"
		<< "Player counts:\nDST : " << all_players[player_position::dst].size () << "\n\n";

// ****************************************************************************************
// Solution using std::async
// ****************************************************************************************
	// Now the fun part... Start building teams.
	std::vector<std::future<std::unique_ptr<team>>> results;
	for (auto& qb : all_players[player_position::qb])
		{
		results.push_back (std::async (std::launch::async, worker, qb, all_players));
		}
	
	// Wait for the threads to join.
	for (auto& future : results)
		{
		future.wait ();
		}


	std::ofstream out_file;
	out_file.open ("data_output.txt", std::ofstream::out | std::ofstream::app);

	for (auto& fut : results)
		{
		auto t = fut.get ();
		std::stringstream str_stream;
		str_stream << "\n\nTWV: '" << t->total_projected_points	<< " TAPPG: '" << t->CalculateTotalAPPG () << "', Salary: '" << t->CalculateTotalSalary () << "'\n" <<
			"QB  : " << t->m_qb.GetName ()   << ", " << t->m_qb.GetProjectedPoints ()   << ", " << t->m_qb.GetAPPG ()   << t->m_qb.GetSalary () << "\n" <<
			"RB1 : " << t->m_rb1.GetName () << ", " << t->m_rb1.GetProjectedPoints () << ", " << t->m_rb1.GetAPPG ()    << t->m_rb1.GetSalary () << "\n" <<
			"RB2 : " << t->m_rb2.GetName () << ", " << t->m_rb2.GetProjectedPoints () << ", " << t->m_rb2.GetAPPG ()    << t->m_rb2.GetSalary () << "\n" <<
			"WR1 : " << t->m_wr1.GetName () << ", " << t->m_wr1.GetProjectedPoints () << ", " << t->m_wr1.GetAPPG ()    << t->m_wr1.GetSalary () << "\n" <<
			"WR2 : " << t->m_wr2.GetName () << ", " << t->m_wr2.GetProjectedPoints () << ", " << t->m_wr2.GetAPPG ()    << t->m_wr2.GetSalary () << "\n" <<
			"WR3 : " << t->m_wr3.GetName () << ", " << t->m_wr3.GetProjectedPoints () << ", " << t->m_wr3.GetAPPG ()    << t->m_wr3.GetSalary () << "\n" <<
			"TE  : " << t->m_te.GetName ()  << ", " << t->m_te.GetProjectedPoints () << ", "   << t->m_te.GetAPPG ()    << t->m_te.GetSalary () << "\n" <<
			"FLEX: " << t->m_flex.GetName () << ", " << t->m_flex.GetProjectedPoints () << ", " << t->m_flex.GetAPPG () << t->m_flex.GetSalary () << "\n" <<
			"DST : " << t->m_dst.GetName () << ", " << t->m_dst.GetProjectedPoints () << ", " << t->m_dst.GetAPPG () << t->m_dst.GetSalary () << "\n";

		std::cout << str_stream.str ();
		out_file << str_stream.str ();
		} 
	
		{
		time_t t = time (0);   // get time now
		struct tm now;
		localtime_s (&now, &t);
		std::cout << std::endl << (now.tm_hour) << ':'
			<< (now.tm_min) << ':'
			<< now.tm_sec
			<< std::endl;
		}

	std::string in;
	std::cin >> in;
    return 0;
	}


// Copyright 2016 Ian Maines
#pragma once

#include "csv.h"

enum class player_position
	{
	qb = 0,
	rb,
	wr,
	te,
	dst,
	flex	// Need to reserve this for the map of players. Not an actual position.
	};

struct dk_player_data_t
	{
	std::string name;
	player_position position;
	size_t salary;
	double appg;
	};

struct rg_player_data_t
	{
	std::string name;
	player_position position;
	std::string team;
	std::string opp_team;
	size_t salary;
	double ceiling;
	double floor;
	double projected_pts;
	};

inline bool is_flex (player_position pos)
	{
	return (pos == player_position::rb ||
			pos == player_position::wr ||
			pos == player_position::te);
	}

class player
	{
	public:
		player (const dk_player_data_t& dk_data, const rg_player_data_t& rg_data);

		player_position GetPosition () const { return m_position; };
		std::string GetName () const { return m_name; }
		size_t GetSalary () const { return m_salary; }
		double GetAPPG () const { return m_appg; }
		size_t GetHash () const { return m_hash; }
		double GetProjectedPoints () const { return m_projected_pts; }

	private:
		player_position m_position;
		std::string m_name;
		size_t m_salary;
		double m_appg;
		double m_projected_pts;	// RotoGrinder projected points.
		double m_pts_per_k;	// Points per $1000
		// Hash makes comparing players easier quicker
		size_t m_hash;
	};


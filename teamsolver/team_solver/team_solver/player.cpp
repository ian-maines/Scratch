// Copyright 2016 Ian Maines
#include "player.h"

namespace
{

}

player::player (const dk_player_data_t& dk_data, const rg_player_data_t& rg_data)
	: m_position (dk_data.position)
	, m_name (dk_data.name)
	, m_salary (dk_data.salary)
	, m_appg (dk_data.appg)
	, m_projected_pts (rg_data.projected_pts)
	, m_pts_per_k (rg_data.projected_pts/(static_cast<double>(dk_data.salary)/1000))
	, m_hash (std::hash<std::string>{}(m_name))
	{
#ifdef _DEBUG
	if (m_position == player_position::flex)
		{
		throw std::exception ();
		}
#endif
	}
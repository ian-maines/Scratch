// Copyright 2016 Ian Maines
#pragma once

#include "globals.h"
#include "player.h"

inline bool is_team_valid (player& qb, player& rb1, player& rb2, player& wr1, player& wr2, player& wr3, player& te, player& flex, player& dst)
	{
#ifdef _DEBUG
	// Player must match position.
	if (qb.GetPosition () != player_position::qb ||
		rb1.GetPosition () != player_position::rb ||
		rb2.GetPosition () != player_position::rb ||
		wr1.GetPosition () != player_position::wr ||
		wr2.GetPosition () != player_position::wr ||
		wr3.GetPosition () != player_position::wr ||
		te.GetPosition () != player_position::te ||
		(flex.GetPosition () != player_position::rb && flex.GetPosition () != player_position::wr && flex.GetPosition () != player_position::te) ||
		dst.GetPosition () != player_position::dst)
		{
		throw std::exception ();
		}
#endif

	// No dupes
	if (rb1.GetHash () == rb2.GetHash () ||
		wr1.GetHash () == wr2.GetHash () ||
		wr1.GetHash () == wr3.GetHash () ||
		wr2.GetHash () == wr3.GetHash () ||
		flex.GetHash () == rb1.GetHash () ||
		flex.GetHash () == rb2.GetHash () ||
		flex.GetHash () == wr1.GetHash () ||
		flex.GetHash () == wr2.GetHash () ||
		flex.GetHash () == wr3.GetHash () ||
		flex.GetHash () == te.GetHash ())
		{
		return false;
		}

	if ((qb.GetSalary () +
		rb1.GetSalary () +
		rb2.GetSalary ()  +
		wr1.GetSalary ()  +
		wr2.GetSalary ()  +
		wr3.GetSalary ()  +
		te.GetSalary ()   +
		flex.GetSalary ()  +
		dst.GetSalary ())
			 > salary_cap)
		{
		return false;
		}
	return true;
	}

struct team
	{
	team (player& qb, player& rb1, player& rb2, player& wr1, player& wr2, player& wr3, player& te, player& flex, player& dst)
		: m_qb (qb)
		, m_rb1 (rb1)
		, m_rb2 (rb2)
		, m_wr1 (wr1)
		, m_wr2 (wr2)
		, m_wr3 (wr3)
		, m_te (te)
		, m_flex (flex)
		, m_dst (dst)
		, total_appg (0)
		, total_salary (0)
		{

		total_appg = qb.GetAPPG () +
			rb1.GetAPPG () +
			rb2.GetAPPG () +
			wr1.GetAPPG () +
			wr2.GetAPPG () +
			wr3.GetAPPG () +
			te.GetAPPG () +
			flex.GetAPPG () +
			dst.GetAPPG ();

		total_salary = qb.GetSalary () +
			rb1.GetSalary () +
			rb2.GetSalary () +
			wr1.GetSalary () +
			wr2.GetSalary () +
			wr3.GetSalary () +
			te.GetSalary () +
			flex.GetSalary () +
			dst.GetSalary ();

		// Validate all inputs
#ifdef _DEBUG
		if (is_team_valid (qb, rb1, rb2, wr1, wr2, wr3, te, flex, dst))
			{
			throw std::exception ();
			}
#endif
		}

	team& operator= (const team& rhs)
		{
		m_qb = rhs.m_qb;
		m_rb1 = rhs.m_rb1;
		m_rb2 = rhs.m_rb2;
		m_wr1 = rhs.m_wr1;
		m_wr2 = rhs.m_wr2;
		m_wr3 = rhs.m_wr3;
		m_te = rhs.m_te;
		m_flex = rhs.m_flex;
		m_dst = rhs.m_dst;
		total_appg = rhs.total_appg;
		total_salary = rhs.total_salary;

		return *this;
		}

	// This is intentionally backwards because priority_queue puts highest first.
	friend bool operator< (const team& lhs, const team& rhs)
		{
		return lhs.total_appg > rhs.total_appg;
		}

	player& m_qb;
	player& m_rb1;
	player& m_rb2;
	player& m_wr1;
	player& m_wr2;
	player& m_wr3;
	player& m_te;
	player& m_flex;
	player& m_dst;

	// For easy player iteration.
	double total_appg;
	size_t total_salary;
	};

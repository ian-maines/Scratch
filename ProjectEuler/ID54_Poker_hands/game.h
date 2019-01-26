// Copyright 2019 Ian Maines
#pragma once

#include "card.h"
#include "evaluator.h"

class CMatch
	{
	public:
		CMatch (const CHand& Player1, const CHand& Player2)
			: m_p1 (Player1)
			, m_p2 (Player2)
			{}

		std::string print () const
			{
			std::stringstream ss;
			ss << m_p1.print () << " | " << m_p2.print ();
			return ss.str ();
			}

		std::string print_result () const
			{
			std::stringstream ss;
			ss << print () << " Winner = " << CEvaluator::print(winner());
			return ss.str();
			}

		CEvaluator::player_t winner () const
			{
			return CEvaluator::CompareHands (m_p1, m_p2);
			}

	private:
		CHand m_p1;
		CHand m_p2;
	};

using tournament_t = std::vector<CMatch>;

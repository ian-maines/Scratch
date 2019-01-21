// Copyright 2019 Ian Maines
#pragma once

#include "card.h"

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
			ss << m_p1.print () << " " << m_p2.print ();
			return ss.str ();
			}

	private:
		CHand m_p1;
		CHand m_p2;
	};

using tournament_t = std::vector<CMatch>;

// Copyright 2019 Ian Maines

#include "pch.h"
#include "..\card.h"

namespace
	{
	TEST (CCard, Constructor)
		{
		CCard c1 (value_t::Ace, suit_t::Spades);
		EXPECT_STREQ (c1.print ().c_str (), "AS");
		CCard c2 (value_t::Two, suit_t::Diamonds);
		EXPECT_STREQ (c2.print().c_str(), "2D");

		}

	TEST (CHand, Constructor)
		{
		CHand::hand_t hand1 = { CCard (value_t::Ace, suit_t::Clubs) };
		EXPECT_THROW(CHand (std::move(hand1)), std::exception);

		CHand::hand_t hand2 = { CCard (value_t::Ace, suit_t::Clubs), CCard (value_t::Three, suit_t::Clubs), CCard (value_t::Five, suit_t::Clubs),
							 CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Nine, suit_t::Clubs) };

		EXPECT_NO_THROW (CHand (std::move (hand2)));
		}
	}
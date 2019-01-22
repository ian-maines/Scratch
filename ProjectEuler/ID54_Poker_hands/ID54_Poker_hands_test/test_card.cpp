// Copyright 2019 Ian Maines

#include "pch.h"
#include "..\card.h"

using namespace card;

namespace
	{
	// Cards in Descending order
	const CCard AS (Ace, Spades);
	const CCard AH (Ace, Hearts);
	const CCard AD (Ace, Diamonds);
	const CCard AC (Ace, Clubs);

	const CCard ThreeD (Three, Diamonds);
	const CCard FiveH (Five, Hearts);
	const CCard JD (Jack, Diamonds);
	const CCard JC (Jack, Clubs);

	TEST (CCard, Constructor)
		{
		CCard c1 (value_t::Ace, suit_t::Spades);
		EXPECT_STREQ (c1.print ().c_str (), "AS");
		CCard c2 (value_t::Two, suit_t::Diamonds);
		EXPECT_STREQ (c2.print().c_str(), "2D");

		}

	TEST (CCard, Comparison_Operators)
		{
		EXPECT_EQ(true, ThreeD < AS);
		EXPECT_EQ(true, ThreeD < FiveH);
		EXPECT_EQ(false, AS < JC);
		}

	TEST (CHand, Constructor)
		{
		// Expect hand size of 5
		EXPECT_THROW(CHand (CHand::hand_t { CCard (value_t::Ace, suit_t::Clubs) }), std::exception);
		EXPECT_NO_THROW (CHand (CHand::hand_t{ CCard (value_t::Ace, suit_t::Clubs), CCard (value_t::Three, suit_t::Clubs), CCard (value_t::Five, suit_t::Clubs),
							 CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Nine, suit_t::Clubs) }));
		EXPECT_THROW (CHand (CHand::hand_t{ CCard (value_t::Ace, suit_t::Clubs), CCard (value_t::Three, suit_t::Clubs), CCard (value_t::Five, suit_t::Clubs),
							 CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Nine, suit_t::Clubs), CCard (value_t::Nine, suit_t::Diamonds) }), std::exception);

		// Expect no duplicate cards in a hand.
		EXPECT_THROW (CHand (CHand::hand_t{ CCard (value_t::Ace, suit_t::Clubs), CCard (value_t::Ace, suit_t::Clubs), CCard (value_t::Five, suit_t::Clubs),
							 CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Nine, suit_t::Clubs) }), std::exception);
		EXPECT_THROW (CHand (CHand::hand_t{ CCard (value_t::Nine, suit_t::Diamonds), CCard (value_t::Nine, suit_t::Diamonds), CCard (value_t::Five, suit_t::Clubs),
							 CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Nine, suit_t::Clubs) }), std::exception);
		EXPECT_NO_THROW (CHand (CHand::hand_t{ CCard (value_t::Nine, suit_t::Diamonds), CCard (value_t::Nine, suit_t::Hearts), CCard (value_t::Nine, suit_t::Spades),
							 CCard (value_t::Nine, suit_t::Clubs), CCard (value_t::Two, suit_t::Clubs) }));
		}

	TEST (card_absolute_compare_t, comparisons)
		{
		card_absolute_compare_t uut;

		EXPECT_EQ (true, uut (AH, AS));
		EXPECT_EQ (true, uut (JC, AS));
		EXPECT_EQ (true, uut (ThreeD, AS));
		EXPECT_EQ (true, uut (ThreeD, FiveH));
		EXPECT_EQ (true, uut (JC, JD));
		
		EXPECT_EQ (false, uut (AS, JD));
		EXPECT_EQ (false, uut (AS, AH));
		}
	}
// Copyright 2019 Ian Maines
#pragma once

#include "pch.h"
#include "..\evaluator.h"

using namespace card;

namespace data
	{
	// Hands for flush testing
	const CHand flush_clubs (CHand::hand_t { CCard (value_t::Ace, suit_t::Clubs), CCard (value_t::Three, suit_t::Clubs), CCard (value_t::Five, suit_t::Clubs),
							 CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Nine, suit_t::Clubs) });
	const CHand flush_Hearts (CHand::hand_t{ CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Three, suit_t::Hearts), CCard (value_t::Five, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Hearts), CCard (value_t::Nine, suit_t::Hearts) });
	const CHand not_flush (CHand::hand_t{ CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Three, suit_t::Hearts), CCard (value_t::Five, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Hearts), CCard (value_t::Nine, suit_t::Spades) });
	// Hands for Royal Flush testing
	const CHand royal_flush_spades (CHand::hand_t{ CCard (value_t::Ace, suit_t::Spades), CCard (value_t::Ten, suit_t::Spades), CCard (value_t::Jack, suit_t::Spades),
							 CCard (value_t::King, suit_t::Spades), CCard (value_t::Queen, suit_t::Spades) });
	const CHand royal_flush_diamonds (CHand::hand_t{ CCard (value_t::Ace, suit_t::Diamonds), CCard (value_t::Ten, suit_t::Diamonds), CCard (value_t::Jack, suit_t::Diamonds),
							 CCard (value_t::King, suit_t::Diamonds), CCard (value_t::Queen, suit_t::Diamonds) });
	const CHand royal_flush_suit_mismatch (CHand::hand_t{ CCard (value_t::Ace, suit_t::Spades), CCard (value_t::Ten, suit_t::Hearts), CCard (value_t::Jack, suit_t::Spades),
							 CCard (value_t::King, suit_t::Spades), CCard (value_t::Queen, suit_t::Spades) });
	// Hands for Straight Flush
	const CHand straight_flush_two_spades (CHand::hand_t{ CCard (value_t::Two, suit_t::Spades), CCard (value_t::Three, suit_t::Spades), CCard (value_t::Five, suit_t::Spades),
							 CCard (value_t::Four, suit_t::Spades), CCard (value_t::Six, suit_t::Spades) });
	const CHand straight_flush_six_hearts (CHand::hand_t{ CCard (value_t::Ten, suit_t::Hearts), CCard (value_t::Six, suit_t::Hearts), CCard (value_t::Nine, suit_t::Hearts),
							 CCard (value_t::Eight, suit_t::Hearts), CCard (value_t::Seven, suit_t::Hearts) });
	const CHand straight_flush_four_diamonds (CHand::hand_t{ CCard (value_t::Eight, suit_t::Diamonds), CCard (value_t::Five, suit_t::Diamonds), CCard (value_t::Four, suit_t::Diamonds),
							 CCard (value_t::Six, suit_t::Diamonds), CCard (value_t::Seven, suit_t::Diamonds) });
	const CHand straight_flush_nine_clubs (CHand::hand_t{ CCard (value_t::King, suit_t::Clubs), CCard (value_t::Nine, suit_t::Clubs), CCard (value_t::Ten, suit_t::Clubs),
							 CCard (value_t::Queen, suit_t::Clubs), CCard (value_t::Jack, suit_t::Clubs) });
	// Hands for 4 of a kind
	const CHand four_oak_kings (CHand::hand_t{ CCard (value_t::King, suit_t::Clubs), CCard (value_t::King, suit_t::Diamonds), CCard (value_t::King, suit_t::Spades),
							 CCard (value_t::King, suit_t::Hearts), CCard (value_t::Jack, suit_t::Clubs) });
	const CHand four_oak_twos (CHand::hand_t{ CCard (value_t::Two, suit_t::Clubs), CCard (value_t::Two, suit_t::Diamonds), CCard (value_t::Two, suit_t::Spades),
							 CCard (value_t::Two, suit_t::Hearts), CCard (value_t::Four, suit_t::Clubs) });
	const CHand four_oak_nines (CHand::hand_t{ CCard (value_t::Nine, suit_t::Clubs), CCard (value_t::Nine, suit_t::Diamonds), CCard (value_t::Nine, suit_t::Spades),
							 CCard (value_t::Nine, suit_t::Hearts), CCard (value_t::Seven, suit_t::Clubs) });

	// Full houses
	const CHand full_house_ka (CHand::hand_t{ CCard (value_t::King, suit_t::Clubs), CCard (value_t::King, suit_t::Diamonds), CCard (value_t::Ace, suit_t::Spades),
							 CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Ace, suit_t::Clubs) });
	const CHand full_house_59 (CHand::hand_t{ CCard (value_t::Five, suit_t::Clubs), CCard (value_t::Five, suit_t::Diamonds), CCard (value_t::Five, suit_t::Spades),
							 CCard (value_t::Nine, suit_t::Hearts), CCard (value_t::Nine, suit_t::Clubs) });
	const CHand full_house_23 (CHand::hand_t{ CCard (value_t::Two, suit_t::Clubs), CCard (value_t::Three, suit_t::Diamonds), CCard (value_t::Two, suit_t::Spades),
							 CCard (value_t::Three, suit_t::Hearts), CCard (value_t::Two, suit_t::Diamonds) });

	// Three of a kinds
	const CHand three_oak_sevens (CHand::hand_t{ CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Seven, suit_t::Spades),
							 CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Two, suit_t::Clubs) });
	const CHand three_oak_fives (CHand::hand_t{ CCard (value_t::Five, suit_t::Clubs), CCard (value_t::Six, suit_t::Diamonds), CCard (value_t::Five, suit_t::Spades),
							 CCard (value_t::Five, suit_t::Hearts), CCard (value_t::Four, suit_t::Clubs) });
	}

namespace
	{
	TEST (CEvaluator, IsFlush)
		{
		EXPECT_EQ (true, CEvaluator::IsFlush (data::flush_clubs));
		EXPECT_EQ (true, CEvaluator::IsFlush (data::flush_Hearts));
		EXPECT_EQ (false, CEvaluator::IsFlush (data::not_flush));
		}

	TEST (CEvaluator, IsStraightFlush)
		{
		// Royal Flushes are technically straight flushes
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::royal_flush_diamonds));
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::royal_flush_spades));
		// Straight-up straight flushes
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_two_spades));
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_six_hearts));
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_four_diamonds));
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_nine_clubs));

		// Not straight flushes
		EXPECT_EQ (false, CEvaluator::IsStraightFlush (data::flush_clubs));
		EXPECT_EQ (false, CEvaluator::IsStraightFlush (data::not_flush));
		}

	TEST (CEvaluator, IsRoyalFlush)
		{
		EXPECT_EQ (true, CEvaluator::IsRoyalFlush (data::royal_flush_spades));
		EXPECT_EQ (true, CEvaluator::IsRoyalFlush (data::royal_flush_diamonds));
		EXPECT_EQ (false, CEvaluator::IsRoyalFlush (data::flush_clubs));
		EXPECT_EQ (false, CEvaluator::IsRoyalFlush (data::not_flush));
		EXPECT_EQ (false, CEvaluator::IsRoyalFlush (data::royal_flush_suit_mismatch));
		EXPECT_EQ (false, CEvaluator::IsRoyalFlush (data::straight_flush_nine_clubs));
		}

	TEST (CEvaluator, Has4OfAKind)
		{
		EXPECT_EQ (true, CEvaluator::Has4OfAKind (data::four_oak_kings));
		EXPECT_EQ (true, CEvaluator::Has4OfAKind (data::four_oak_twos));
		EXPECT_EQ (true, CEvaluator::Has4OfAKind (data::four_oak_nines));

		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::royal_flush_diamonds));
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::straight_flush_four_diamonds));
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::not_flush));
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::flush_clubs));
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::royal_flush_suit_mismatch));
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::full_house_59));
		}

	TEST (CEvaluator, Has3OfAKind)
		{
		// Technically 4OAKs are 3OAKs
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::four_oak_kings));
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::four_oak_twos));
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::four_oak_nines));
		// So are full houses
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::full_house_ka));
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::full_house_59));
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::full_house_23));
		// Actual three of a kinds
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::three_oak_fives));
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::three_oak_sevens));

		// Not
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::royal_flush_diamonds));
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::not_flush));
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::flush_Hearts));
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::straight_flush_six_hearts));
		}
	}
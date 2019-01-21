// Copyright 2019 Ian Maines
#pragma once

#include "pch.h"
#include "..\evaluator.h"

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
		EXPECT_EQ (true, CEvaluator::IsStraightFlush(data::royal_flush_diamonds));
		EXPECT_EQ (false, CEvaluator::IsStraightFlush (data::flush_clubs));
		}

	TEST (CEvaluator, IsRoyalFlush)
		{
		EXPECT_EQ (true, CEvaluator::IsRoyalFlush (data::royal_flush_spades));
		EXPECT_EQ (true, CEvaluator::IsRoyalFlush (data::royal_flush_diamonds));
		EXPECT_EQ (false, CEvaluator::IsRoyalFlush (data::flush_clubs));
		EXPECT_EQ (false, CEvaluator::IsRoyalFlush (data::not_flush));
		EXPECT_EQ (false, CEvaluator::IsRoyalFlush (data::royal_flush_suit_mismatch));
		}
	}
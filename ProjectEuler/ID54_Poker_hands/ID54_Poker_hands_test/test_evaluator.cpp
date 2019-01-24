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
	const CHand flush_hearts (CHand::hand_t{ CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Three, suit_t::Hearts), CCard (value_t::Five, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Hearts), CCard (value_t::Nine, suit_t::Hearts) });
	const CHand flush_diamonds_AT753 (CHand::hand_t{ CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Ten, suit_t::Hearts), CCard (value_t::Seven, suit_t::Hearts),
							 CCard (value_t::Five, suit_t::Hearts), CCard (value_t::Three, suit_t::Hearts) });
	const CHand flush_diamonds_AT754 (CHand::hand_t{ CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Ten, suit_t::Hearts), CCard (value_t::Seven, suit_t::Hearts),
							 CCard (value_t::Five, suit_t::Hearts), CCard (value_t::Four, suit_t::Hearts) });
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
							 CCard (value_t::Two, suit_t::Hearts), CCard (value_t::Ace, suit_t::Clubs) });
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
	const CHand three_oak_sevens_A2 (CHand::hand_t{ CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Seven, suit_t::Spades),
						 CCard (value_t::Ace, suit_t::Hearts), CCard (value_t::Two, suit_t::Clubs) });
	const CHand three_oak_sevens_J6 (CHand::hand_t{ CCard (value_t::Seven, suit_t::Clubs), CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Seven, suit_t::Spades),
						 CCard (value_t::Jack, suit_t::Hearts), CCard (value_t::Six, suit_t::Clubs) });
	const CHand three_oak_fives (CHand::hand_t{ CCard (value_t::Five, suit_t::Clubs), CCard (value_t::Six, suit_t::Diamonds), CCard (value_t::Five, suit_t::Spades),
							 CCard (value_t::Five, suit_t::Hearts), CCard (value_t::Four, suit_t::Clubs) });

	// Two Pair
	const CHand two_pair_2s_5s_6 (CHand::hand_t{ CCard (value_t::Two, suit_t::Spades), CCard (value_t::Five, suit_t::Spades), CCard (value_t::Five, suit_t::Hearts),
							 CCard (value_t::Two, suit_t::Diamonds), CCard (value_t::Six, suit_t::Spades) });
	const CHand two_pair_2s_5s_8 (CHand::hand_t{ CCard (value_t::Two, suit_t::Spades), CCard (value_t::Five, suit_t::Spades), CCard (value_t::Five, suit_t::Hearts),
							 CCard (value_t::Two, suit_t::Diamonds), CCard (value_t::Eight, suit_t::Spades) });
	const CHand two_pair_3s_5s_8 (CHand::hand_t{ CCard (value_t::Three, suit_t::Spades), CCard (value_t::Five, suit_t::Spades), CCard (value_t::Five, suit_t::Hearts),
							 CCard (value_t::Three, suit_t::Diamonds), CCard (value_t::Eight, suit_t::Spades) });
	const CHand two_pair_As_Js (CHand::hand_t{ CCard (value_t::Ace, suit_t::Spades), CCard (value_t::Jack, suit_t::Spades), CCard (value_t::Jack, suit_t::Hearts),
							 CCard (value_t::Ace, suit_t::Diamonds), CCard (value_t::Nine, suit_t::Spades) });
	const CHand two_pair_Qs_5s (CHand::hand_t{ CCard (value_t::Queen, suit_t::Spades), CCard (value_t::Five, suit_t::Spades), CCard (value_t::Five, suit_t::Hearts),
							 CCard (value_t::Queen, suit_t::Diamonds), CCard (value_t::Jack, suit_t::Spades) });

	// Pair
	const CHand pair_9s (CHand::hand_t{ CCard (value_t::Nine, suit_t::Spades), CCard (value_t::Five, suit_t::Spades), CCard (value_t::Nine, suit_t::Hearts),
							 CCard (value_t::Two, suit_t::Diamonds), CCard (value_t::Six, suit_t::Spades) });
	const CHand pair_As (CHand::hand_t{ CCard (value_t::Ace, suit_t::Spades), CCard (value_t::Jack, suit_t::Spades), CCard (value_t::Ace, suit_t::Hearts),
							CCard (value_t::Two, suit_t::Diamonds), CCard (value_t::Three, suit_t::Spades) });
	const CHand pair_3s (CHand::hand_t{ CCard (value_t::Queen, suit_t::Spades), CCard (value_t::Five, suit_t::Spades), CCard (value_t::Three, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Three, suit_t::Spades) });
	// Straight
	const CHand straight_3 (CHand::hand_t{ CCard (value_t::Five, suit_t::Spades), CCard (value_t::Three, suit_t::Diamonds), CCard (value_t::Six, suit_t::Hearts),
							 CCard (value_t::Four, suit_t::Diamonds), CCard (value_t::Seven, suit_t::Spades) });
	const CHand straight_t (CHand::hand_t{ CCard (value_t::Ace, suit_t::Spades), CCard (value_t::Jack, suit_t::Diamonds), CCard (value_t::Queen, suit_t::Hearts),
							 CCard (value_t::Ten, suit_t::Diamonds), CCard (value_t::King, suit_t::Spades) });
	const CHand straight_7 (CHand::hand_t{ CCard (value_t::Jack, suit_t::Spades), CCard (value_t::Eight, suit_t::Diamonds), CCard (value_t::Seven, suit_t::Hearts),
							 CCard (value_t::Ten, suit_t::Diamonds), CCard (value_t::Nine, suit_t::Spades) });

	// High Card
	const CHand high_card_ace_ten_seven (CHand::hand_t{ CCard (value_t::Five, suit_t::Spades), CCard (value_t::Three, suit_t::Diamonds), CCard (value_t::Ace, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Ten, suit_t::Spades) });
	const CHand high_card_eight_seven_five (CHand::hand_t{ CCard (value_t::Five, suit_t::Spades), CCard (value_t::Three, suit_t::Diamonds), CCard (value_t::Eight, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Two, suit_t::Spades) });
	const CHand high_card_jack_eight_seven (CHand::hand_t{ CCard (value_t::Five, suit_t::Spades), CCard (value_t::Three, suit_t::Diamonds), CCard (value_t::Jack, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Eight, suit_t::Spades) });
	const CHand high_card_ace_ten_seven_five_four (CHand::hand_t{ CCard (value_t::Five, suit_t::Spades), CCard (value_t::Four, suit_t::Diamonds), CCard (value_t::Ace, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Ten, suit_t::Spades) });
	const CHand high_card_ace_ten_seven_five_three (CHand::hand_t{ CCard (value_t::Five, suit_t::Spades), CCard (value_t::Three, suit_t::Diamonds), CCard (value_t::Ace, suit_t::Hearts),
							 CCard (value_t::Seven, suit_t::Diamonds), CCard (value_t::Ten, suit_t::Spades) });
	}

namespace
	{
	TEST (CEvaluator, IsFlush)
		{
		EXPECT_EQ (true, CEvaluator::IsFlush (data::flush_clubs).bIsFlush);
		EXPECT_EQ (Ace, CEvaluator::IsFlush (data::flush_clubs).sorted_cards.back());
		EXPECT_EQ (true, CEvaluator::IsFlush (data::flush_hearts).bIsFlush);
		EXPECT_EQ (false, CEvaluator::IsFlush (data::not_flush).bIsFlush);
		}

	TEST (CEvaluator, IsStraightFlush)
		{
		// Royal Flushes are technically straight flushes
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::royal_flush_diamonds).bIsStraightFlush);
		EXPECT_EQ (Ace, CEvaluator::IsStraightFlush (data::royal_flush_diamonds).high_card);
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::royal_flush_spades).bIsStraightFlush);
		// Straight-up straight flushes
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_two_spades).bIsStraightFlush);
		EXPECT_EQ (Six, CEvaluator::IsStraightFlush (data::straight_flush_two_spades).high_card);
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_six_hearts).bIsStraightFlush);
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_four_diamonds).bIsStraightFlush);
		EXPECT_EQ (Eight, CEvaluator::IsStraightFlush (data::straight_flush_four_diamonds).high_card);
		EXPECT_EQ (true, CEvaluator::IsStraightFlush (data::straight_flush_nine_clubs).bIsStraightFlush);

		// Not straight flushes
		EXPECT_EQ (false, CEvaluator::IsStraightFlush (data::flush_clubs).bIsStraightFlush);
		EXPECT_EQ (false, CEvaluator::IsStraightFlush (data::not_flush).bIsStraightFlush);
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
		EXPECT_EQ (true, CEvaluator::Has4OfAKind (data::four_oak_kings).bHas4oak);
		EXPECT_EQ (King, CEvaluator::Has4OfAKind (data::four_oak_kings).four_oak_card);
		EXPECT_EQ (Jack, CEvaluator::Has4OfAKind (data::four_oak_kings).high_card);
		EXPECT_EQ (true, CEvaluator::Has4OfAKind (data::four_oak_twos).bHas4oak);
		EXPECT_EQ (true, CEvaluator::Has4OfAKind (data::four_oak_nines).bHas4oak);

		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::royal_flush_diamonds).bHas4oak);
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::straight_flush_four_diamonds).bHas4oak);
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::not_flush).bHas4oak);
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::flush_clubs).bHas4oak);
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::royal_flush_suit_mismatch).bHas4oak);
		EXPECT_EQ (false, CEvaluator::Has4OfAKind (data::full_house_59).bHas4oak);
		}

	TEST (CEvaluator, Has3OfAKind)
		{
		// Three of a kinds
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::three_oak_fives).bHas3Oak);
		EXPECT_EQ (Six, CEvaluator::Has3OfAKind (data::three_oak_fives).sorted_rem_cards.back());
		EXPECT_EQ (Five, CEvaluator::Has3OfAKind (data::three_oak_fives).three_cards_value);
		EXPECT_EQ (true, CEvaluator::Has3OfAKind (data::three_oak_sevens).bHas3Oak);

		// Not
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::royal_flush_diamonds).bHas3Oak);
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::not_flush).bHas3Oak);
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::flush_hearts).bHas3Oak);
		EXPECT_EQ (false, CEvaluator::Has3OfAKind (data::straight_flush_six_hearts).bHas3Oak);
		}

	TEST (CEvaluator, HasPair)
		{
		// pair
		EXPECT_EQ (true, CEvaluator::HasPair (data::pair_3s).bHasPair);
		EXPECT_EQ (Three, CEvaluator::HasPair (data::pair_3s).pair_value);
		EXPECT_EQ (Queen, CEvaluator::HasPair (data::pair_3s).sorted_rem_cards.back());
		EXPECT_EQ (true, CEvaluator::HasPair (data::pair_9s).bHasPair);
		EXPECT_EQ (true, CEvaluator::HasPair (data::pair_As).bHasPair);

		// Not
		EXPECT_EQ (false, CEvaluator::HasPair (data::royal_flush_diamonds).bHasPair);
		EXPECT_EQ (false, CEvaluator::HasPair (data::not_flush).bHasPair);
		EXPECT_EQ (false, CEvaluator::HasPair (data::flush_hearts).bHasPair);
		EXPECT_EQ (false, CEvaluator::HasPair (data::straight_flush_six_hearts).bHasPair);
		}

	TEST (CEvaluator, HasTwoPair)
		{
		// actual two pairs
		EXPECT_EQ (true, CEvaluator::HasTwoPair (data::two_pair_2s_5s_6).bHasTwoPair);
		EXPECT_EQ (Six, CEvaluator::HasTwoPair (data::two_pair_2s_5s_6).high_card);
		EXPECT_EQ (Five, CEvaluator::HasTwoPair (data::two_pair_2s_5s_6).high_pair);
		EXPECT_EQ (Two, CEvaluator::HasTwoPair (data::two_pair_2s_5s_6).low_pair);
		EXPECT_EQ (true, CEvaluator::HasTwoPair (data::two_pair_As_Js).bHasTwoPair);
		EXPECT_EQ (true, CEvaluator::HasTwoPair (data::two_pair_Qs_5s).bHasTwoPair);

		// Not
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::royal_flush_diamonds).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::not_flush).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::flush_hearts).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::straight_flush_six_hearts).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::four_oak_kings).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::four_oak_twos).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::four_oak_nines).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::pair_3s).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::pair_9s).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::pair_As).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::three_oak_fives).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::three_oak_sevens).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::full_house_ka).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::full_house_59).bHasTwoPair);
		EXPECT_EQ (false, CEvaluator::HasTwoPair (data::full_house_23).bHasTwoPair);
		}
	
	TEST (CEvaluator, IsStraight)
		{
		EXPECT_EQ (true, CEvaluator::IsStraight (data::straight_3).bIsStraight);
		EXPECT_EQ (Seven, CEvaluator::IsStraight (data::straight_3).high_card);
		EXPECT_EQ (true, CEvaluator::IsStraight (data::straight_t).bIsStraight);
		EXPECT_EQ (true, CEvaluator::IsStraight (data::straight_7).bIsStraight);

		EXPECT_EQ (true, CEvaluator::IsStraight (data::royal_flush_spades).bIsStraight);
		EXPECT_EQ (true, CEvaluator::IsStraight (data::straight_flush_nine_clubs).bIsStraight);

		// Not
		EXPECT_EQ (false, CEvaluator::IsStraight (data::not_flush).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::flush_hearts).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::four_oak_kings).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::four_oak_twos).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::four_oak_nines).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::pair_3s).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::pair_9s).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::pair_As).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::three_oak_fives).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::three_oak_sevens).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::full_house_ka).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::full_house_59).bIsStraight);
		EXPECT_EQ (false, CEvaluator::IsStraight (data::full_house_23).bIsStraight);
		}

	TEST (CEvaluator, CompareHands)
		{
		// ** TEST NOTE **
		// Tests start down at Royal flush, which compares itself against all other hand types.
		// Each hand type working up does not compare to the type below it, as those combinations of hand types have already been tested.

		// High Card : Highest value card.
			// Tested against all other hand types later on.
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::high_card_ace_ten_seven_five_four, data::high_card_ace_ten_seven_five_three));
		// One Pair : Two cards of the same value.

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::pair_9s, data::high_card_ace_ten_seven));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_eight_seven_five, data::pair_As));
		// Two Pairs : Two different pairs.s
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::two_pair_2s_5s_8));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::two_pair_3s_5s_8, data::two_pair_2s_5s_8));

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::high_card_ace_ten_seven));
		// reverses
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::two_pair_2s_5s_6));

		// Three of a Kind : Three cards of the same value.
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::three_oak_sevens_A2, data::three_oak_sevens_J6));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::three_oak_sevens_J6, data::three_oak_sevens));

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::three_oak_sevens, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::three_oak_sevens, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::three_oak_sevens, data::high_card_ace_ten_seven));
		// reverses
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::three_oak_sevens));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::three_oak_sevens));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::three_oak_sevens));

		// Straight : All cards are consecutive values.
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_t, data::straight_7));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_3, data::straight_7));

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_t, data::three_oak_fives));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_t, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_t, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_t, data::high_card_ace_ten_seven));
		// reverses
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::three_oak_fives, data::straight_t));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::straight_t));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::straight_t));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::straight_t));

		// Flush : All cards of the same suit.
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::flush_diamonds_AT753, data::flush_diamonds_AT754));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::flush_diamonds_AT753, data::flush_clubs));

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::flush_hearts, data::straight_3));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::flush_hearts, data::three_oak_fives));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::flush_hearts, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::flush_hearts, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::flush_hearts, data::high_card_ace_ten_seven));
		// reverse
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_3, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::three_oak_fives, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::full_house_ka));


		// Full House : Three of a kind and a pair.
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::full_house_ka, data::full_house_23));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::full_house_23, data::full_house_59));

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::full_house_ka, data::flush_clubs));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::full_house_ka, data::straight_3));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::full_house_ka, data::three_oak_fives));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::full_house_ka, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::full_house_ka, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::full_house_ka, data::high_card_ace_ten_seven));
		// reverse
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::flush_clubs, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_3, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::three_oak_fives, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::full_house_ka));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::full_house_ka));

		// Four of a Kind : Four cards of the same value.
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::four_oak_nines));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::four_oak_nines, data::four_oak_kings));

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::full_house_23));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::flush_clubs));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::straight_3));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::three_oak_fives));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::four_oak_twos, data::high_card_ace_ten_seven));
		// reverse
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::full_house_23, data::four_oak_twos));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::flush_clubs, data::four_oak_twos));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_3, data::four_oak_twos));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::three_oak_fives, data::four_oak_twos));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::four_oak_twos));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::four_oak_twos));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::four_oak_twos));

		// Straight Flush : All cards are consecutive values of same suit.
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_flush_two_spades, data::straight_flush_four_diamonds));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::straight_flush_four_diamonds));

		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::four_oak_kings));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::full_house_23));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::flush_clubs));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::straight_3));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::three_oak_fives));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::straight_flush_two_spades, data::high_card_ace_ten_seven));
		// reverse
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::four_oak_kings, data::straight_flush_six_hearts));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::full_house_23, data::straight_flush_six_hearts));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::flush_clubs, data::straight_flush_six_hearts));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_3, data::straight_flush_six_hearts));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::three_oak_fives, data::straight_flush_six_hearts));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::straight_flush_six_hearts));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::straight_flush_six_hearts));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::straight_flush_six_hearts));


		// Royal Flush : Ten, Jack, Queen, King, Ace, in same suit.
		// Don't have instructions on what happens when you get two royal flushes
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::straight_flush_four_diamonds));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::four_oak_kings));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::full_house_23));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::flush_clubs));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::straight_3));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::three_oak_fives));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::two_pair_2s_5s_6));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::pair_3s));
		EXPECT_EQ (CEvaluator::player_1, CEvaluator::CompareHands (data::royal_flush_diamonds, data::high_card_ace_ten_seven));
		// reverse
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_flush_four_diamonds, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::four_oak_kings, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::full_house_23, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::flush_clubs, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::straight_3, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::three_oak_fives, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::two_pair_2s_5s_6, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::pair_3s, data::royal_flush_diamonds));
		EXPECT_EQ (CEvaluator::player_2, CEvaluator::CompareHands (data::high_card_ace_ten_seven, data::royal_flush_diamonds));
		}
	}
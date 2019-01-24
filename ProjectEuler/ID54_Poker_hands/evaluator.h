// Copyright 2019 Ian Maines
#pragma once

#include <algorithm>
#include <set>
#include <functional>

#include "card.h"

// TODO: Hand valuation.
// We need a way to organize the rules for how a hand wins in such a way that judging hands is fairly clear and straightforward

// Take one: start at the highest value hand and work downward:
/*
High Card : Highest value card.
One Pair : Two cards of the same value.
Two Pairs : Two different pairs.
Three of a Kind : Three cards of the same value.
Straight : All cards are consecutive values.
Flush : All cards of the same suit.
Full House : Three of a kind and a pair.
Four of a Kind : Four cards of the same value.
Straight Flush : All cards are consecutive values of same suit.
Royal Flush : Ten, Jack, Queen, King, Ace, in same suit.
*/
using namespace card;

class CEvaluator
	{
	public:
		// At present we don't have handling for a tie with royal flushes
		static bool IsRoyalFlush (const CHand& hand);

		struct str_flush_t
			{
			operator bool () {return bIsStraightFlush;}
			bool bIsStraightFlush = false;
			value_t high_card = Two;
			};
		static const str_flush_t IsStraightFlush (const CHand& hand);

		struct four_oak_t
			{
			operator bool () { return bHas4oak; }
			bool bHas4oak = false;
			value_t high_card = Two;
			value_t four_oak_card = Two;
			};
		static const four_oak_t Has4OfAKind (const CHand& hand);

		struct full_house_t
			{
			operator bool () { return bIsFullHouse; }
			bool bIsFullHouse = false;
			value_t three_value = Two;
			};
		static const full_house_t IsFullHouse (const CHand& hand);

		struct flush_t
			{
			operator bool () { return bIsFlush; }
			bool bIsFlush = false;
			// Highest value card at the end.
			std::vector<value_t> sorted_cards;
			};
		static const flush_t IsFlush (const CHand& hand);

		struct straight_t
			{
			operator bool () { return bIsStraight; }
			bool bIsStraight = false;
			value_t high_card = Two;
			};
		static const straight_t IsStraight (const CHand& hand);

		struct three_oak_t
			{
			operator bool () { return bHas3Oak; }
			bool bHas3Oak = false;
			value_t three_cards_value = Two;
			// Only the two cards that aren't part of the three of a kind, highest value card at the end.
			std::vector<value_t> sorted_rem_cards;
			};
		static const three_oak_t Has3OfAKind (const CHand& hand);

		struct two_pair_t
			{
			operator bool () { return bHasTwoPair; }
			bool bHasTwoPair = false;
			value_t high_pair = Three;
			value_t low_pair = Two;
			value_t high_card = Four;
			};
		static const two_pair_t HasTwoPair (const CHand& hand);

		struct pair_t
			{
			operator bool () { return bHasPair; }
			bool bHasPair = false;
			value_t pair_value = Two;
			// Only the two cards that aren't part of the pair, highest value card at the end.
			std::vector<value_t> sorted_rem_cards;
			};
		static const pair_t HasPair (const CHand& hand);
		
		enum player_t
			{
			player_1 = 1,
			player_2,
			};
		static player_t CompareHands (const CHand& player1, const CHand& player2);
		
		// Expects vector size to be equal. Throws exception if not.
		// Return designates which player has high card. Exception thrown on tie.
		static player_t PlayerWithHighCard (std::vector<value_t> player1, std::vector<value_t> player2);

	private:
		struct has_xoak
			{
			bool bHas = false;
			value_t xoak = Two;
			};
		static const has_xoak _HasXOfAKind (const CHand& hand, int number);
		// Returns sorted vector with highest value card at the end. Include predicate which returns true on values it wants included in the final sorted list
		static const std::vector<value_t> _GetSortedValueSet (const CHand& hand, std::function<bool(value_t)> pred = [](value_t v) { return true; });
	};
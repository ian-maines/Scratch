// Copyright 2019 Ian Maines
#pragma once

#include <algorithm>
#include <set>

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
		static bool IsFlush (const CHand& hand);
		static bool IsStraightFlush (const CHand& hand);
		static bool IsRoyalFlush (const CHand& hand);
	};
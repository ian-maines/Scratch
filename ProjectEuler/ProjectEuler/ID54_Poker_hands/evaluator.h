// Copyright 2019 Ian Maines
#pragma once

#include <algorithm>

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

class CEvaluator
	{
	public:


	private:
		bool _IsRoyalFlush (const CHand& hand)
			{
			if (_IsFlush (hand))
				{

				}
			return false;
			}

		bool _IsFlush (const CHand& hand)
			{
			bool bFlush = true;
			const CHand::hand_t& h (hand.get ());
			suit_t last_suit = h.front ().GetSuit ();
			std::for_each (h.begin (), h.end (), [&bFlush, &last_suit](const CCard& card)
				{
				if (card.GetSuit () != last_suit)
					{
					bFlush = false;
					}
				last_suit = card.GetSuit ();
				});

			return bFlush;
			}
	};
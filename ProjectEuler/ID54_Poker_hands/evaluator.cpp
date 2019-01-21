// Copyright 2019 Ian Maines
#include "pch.h"
#include "evaluator.h"

bool CEvaluator::IsFlush (const CHand& hand)
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

bool CEvaluator::IsStraightFlush (const CHand& hand)
	{
	CHand::hand_t h = hand;

	std::sort (h.begin (), h.end (), []() 
		{
		});
	}

bool CEvaluator::IsRoyalFlush (const CHand& hand)
	{
	if (IsFlush (hand))
		{
		// All cards same suit, now just make sure we have all of the cards accounted for.
		// std::set is a good way to do this.
		std::set<value_t> cards;
		const CHand::hand_t& h = hand.get ();
		std::for_each (h.begin (), h.end (), [&cards](const CCard& c)
			{
			switch (c.GetValue ())
				{
				case (value_t::Ten):
				case (value_t::Jack):
				case (value_t::Queen):
				case (value_t::King):
				case (value_t::Ace):
					cards.insert (c.GetValue ());
					break;
				default:
					break;
				}
			});
		// Set won't accept duplicates, so if size is 5, we have a royal flush.
		if (cards.size () == 5)
			{
			return true;
			}
		}
	return false;
	}
// Copyright 2019 Ian Maines
#include "pch.h"
#include "evaluator.h"

using namespace card;

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
	if (IsFlush (hand))
		{
		CHand::hand_t h = hand.get();
		std::sort (h.begin (), h.end ());	// Rely on operator< defined in card.h
		
		bool bStraight = true;
		for (size_t i = 0; i < h.size () - 1; ++i)
			{
			// Find the iterator to the current and next cards as they appear in the value order list.
			auto i_cur = std::find(value_order.begin(), value_order.end(), h[i].GetValue());
			auto i_next = std::find (value_order.begin (), value_order.end (), h[i+1].GetValue ());
			if (i_cur == value_order.end () || i_next == value_order.end ())
				{
				throw std::exception ("Unexpected value");
				}
			if (*(i_cur + 1) != *i_next)
				{
				bStraight = false;
				}
			}
		return bStraight;
		}
	return false;
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

bool CEvaluator::Has4OfAKind (const CHand& hand)
	{
	std::set<value_t> values;

	const CHand::hand_t& h = hand.get ();
	std::for_each (h.begin (), h.end (), [&values](const CCard& c)
		{
		values.insert(c.GetValue ());
		});

	// In a hand with a 4-of-a-kind, there will only be two different values in the hand: 4 of whatever the four-of-a-kind is, and one for whatever the remaining card is.
	return values.size () == 2;
	}
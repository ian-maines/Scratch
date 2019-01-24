// Copyright 2019 Ian Maines
#include "pch.h"
#include <map>
#include <algorithm>
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

bool CEvaluator::IsStraight (const CHand& hand)
	{
	CHand::hand_t h = hand.get ();
	std::sort (h.begin (), h.end ());	// Rely on operator< defined in card.h

	bool bStraight = true;
	for (size_t i = 0; i < h.size () - 1; ++i)
		{
		// Find the iterator to the current and next cards as they appear in the value order list.
		auto i_cur = std::find (value_order.begin (), value_order.end (), h[i].GetValue ());
		auto i_next = std::find (value_order.begin (), value_order.end (), h[i + 1].GetValue ());
		if (i_cur == value_order.end () || i_next == value_order.end ())
			{
			throw std::exception ("Unexpected value");
			}
		// If we have a pair of aces, we could have i_cur be the last element in the array, so we don't want to advance it
		if (i_cur+1 == value_order.end() || *(i_cur + 1) != *i_next)
			{
			bStraight = false;
			}
		}
	return bStraight;
	}

bool CEvaluator::IsStraightFlush (const CHand& hand)
	{
	return IsStraight (hand) && IsFlush (hand);
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
	return _HasXOfAKind (hand, 4);
	}

bool CEvaluator::IsFullHouse (const CHand& hand)
	{
	// We can solve this in a similar manner to _HasXOfAKind
	std::map<value_t, int> values;

	const CHand::hand_t& h = hand.get ();
	std::for_each (h.begin (), h.end (), [&values](const CCard& c)
		{
		if (values.find (c.GetValue ()) == values.end ())
			{
			values.insert (std::make_pair (c.GetValue (), 1));
			}
		else
			{
			values[c.GetValue ()]++;
			}
		});

	bool b3OAK = std::any_of (values.begin (), values.end (), [](const std::pair<value_t, int>& val) {return val.second == 3; });
	bool bPair = std::any_of (values.begin (), values.end (), [](const std::pair<value_t, int>& val) {return val.second == 2; });

	return b3OAK && bPair;
	}

bool CEvaluator::Has3OfAKind (const CHand& hand)
	{
	return _HasXOfAKind (hand, 3);
	}

bool CEvaluator::HasTwoPair (const CHand& hand)
	{
	// we can do this similar to _HasXOfAKind and IsFullHouse
	std::map<value_t, int> values;

	const CHand::hand_t& h = hand.get ();
	std::for_each (h.begin (), h.end (), [&values](const CCard& c)
		{
		if (values.find (c.GetValue ()) == values.end ())
			{
			values.insert (std::make_pair (c.GetValue (), 1));
			}
		else
			{
			values[c.GetValue ()]++;
			}
		});

	size_t pair_count = 0;

	std::for_each (values.begin (), values.end (), [&pair_count](const std::pair<value_t, int>& val) {if (val.second == 2) { pair_count++; } });

	return pair_count == 2;
	}

bool CEvaluator::HasPair (const CHand& hand)
	{
	return _HasXOfAKind (hand, 2);
	}

CEvaluator::player_t CEvaluator::CompareHands (const CHand& player1, const CHand& player2)
	{
	// FIXME: Is there a better way than brute forcing?
	//Royal flush
		{
		const bool p1rf = IsRoyalFlush (player1);
		const bool p2rf = IsRoyalFlush (player2);
		if (p1rf && p2rf)
			{
			throw std::exception ("Unclear rules for two royal flushes");
			}
		if (p1rf) { return player_1; }
		if (p2rf) { return player_2; }
		}
		
	// Straight flush
		{
		const bool p1sf = IsStraightFlush (player1);
		const bool p2sf = IsStraightFlush (player2);
		if (p1sf && p2sf)
			{
			throw std::exception ("Unclear rules for two straight flushes");
			}
		if (p1sf) { return player_1; }
		if (p2sf) { return player_2; }
		}

	// Four of a kind (high card wins tie, high set of 4 wins tie of that)
		{
		const bool p14oak = Has4OfAKind (player1);
		const bool p24oak = Has4OfAKind (player2);
		if (p14oak && p24oak)
			{
			// FIXME!
			throw std::exception ("Not implemented");
			}
		if (p14oak) { return player_1; }
		if (p24oak) { return player_2; }
		}

	// Full House : Three of a kind and a pair. (high three of a kind wins tie)
		{
		const bool p1fh = IsFullHouse (player1);
		const bool p2fh = IsFullHouse (player2);
		if (p1fh && p2fh)
			{
			throw std::exception ("Not implemented");
			}
		if (p1fh) { return player_1; }
		if (p2fh) { return player_2; }		
		}

	// Flush : All cards of the same suit.
		{
		const bool p1flush = IsFlush (player1);
		const bool p2flush = IsFlush (player2);
		if (p1flush && p2flush)
			{
			throw std::exception("not implemented");
			}
		if (p1flush) { return player_1; }
		if (p2flush) { return player_2; }
		}
		
	// Straight : All cards are consecutive values.
		{
		const bool p1straight = IsStraight (player1);
		const bool p2straight = IsStraight (player2);
		
		if (p1straight && p2straight)
			{
			throw std::exception ("Not implemented");
			}
		
		if (p1straight) { return player_1; }
		if (p2straight) { return player_2; }
		}

	// Three of a Kind : Three cards of the same value.
		{
		const bool p13oak = Has3OfAKind (player1);
		const bool p23oak = Has3OfAKind (player2);

		if (p13oak && p23oak)
			{
			throw std::exception ("Not implemented");
			}

		if (p13oak) { return player_1; }
		if (p23oak) { return player_2; }
		}

	// Two Pairs : Two different pairs.
		{
		const bool p12pair = HasTwoPair (player1);
		const bool p22pair = HasTwoPair (player2);

		if (p12pair && p22pair)
			{
			throw std::exception("Not implemented");
			}

		if (p12pair) { return player_1; }
		if (p22pair) { return player_2; }
		}

	// One Pair : Two cards of the same value.
		{
		const bool p1pair = HasPair (player1);
		const bool p2pair = HasPair (player2);

		if (p1pair && p2pair)
			{
			throw std::exception ("Not implemented");
			}

		if (p1pair) { return player_1; }
		if (p2pair) { return player_2; }
		}

	// High Card : Highest value card.
	}

bool CEvaluator::_HasXOfAKind (const CHand& hand, int number)
	{
	std::map<value_t, int> values;

	const CHand::hand_t& h = hand.get ();
	std::for_each (h.begin (), h.end (), [&values](const CCard& c)
		{
		if (values.find (c.GetValue ()) == values.end ())
			{
			values.insert (std::make_pair (c.GetValue (), 1));
			}
		else
			{
			values[c.GetValue ()]++;
			}
		});

	return std::any_of (values.begin (), values.end (), [&number](const std::pair<value_t, int>& val) {return val.second >= number; });
	}
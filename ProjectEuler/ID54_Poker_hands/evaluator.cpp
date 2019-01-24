// Copyright 2019 Ian Maines
#include "pch.h"
#include <map>
#include <algorithm>
#include "evaluator.h"

using namespace card;

const CEvaluator::flush_t CEvaluator::IsFlush (const CHand& hand)
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

	return flush_t { bFlush, _GetSortedValueSet (hand) };
	}

const CEvaluator::straight_t CEvaluator::IsStraight (const CHand& hand)
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
	return straight_t{ bStraight, _GetSortedValueSet(hand).back() };
	}

const CEvaluator::str_flush_t CEvaluator::IsStraightFlush (const CHand& hand)
	{
	bool bIsStraightFlush (IsStraight (hand).bIsStraight && IsFlush (hand).bIsFlush);

	return { bIsStraightFlush, Two };
	}

bool CEvaluator::IsRoyalFlush (const CHand& hand)
	{
	if (IsFlush (hand).bIsFlush)
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

const CEvaluator::four_oak_t CEvaluator::Has4OfAKind (const CHand& hand)
	{
	return { _HasXOfAKind (hand, 4), Two, Three };
	}

const CEvaluator::full_house_t CEvaluator::IsFullHouse (const CHand& hand)
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

	return { b3OAK && bPair, Two };
	}

const CEvaluator::three_oak_t CEvaluator::Has3OfAKind (const CHand& hand)
	{
	return { _HasXOfAKind (hand, 3), Two, std::vector<value_t> () };
	}

const CEvaluator::two_pair_t CEvaluator::HasTwoPair (const CHand& hand)
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

	return { pair_count == 2, Three, Two, Four };
	}

const CEvaluator::pair_t CEvaluator::HasPair (const CHand& hand)
	{
	return { _HasXOfAKind (hand, 2), Two, std::vector<value_t> () };
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
		const auto p1sf = IsStraightFlush (player1);
		const auto p2sf = IsStraightFlush (player2);
		if (p1sf.bIsStraightFlush && p2sf.bIsStraightFlush)
			{
			throw std::exception ("Unclear rules for two straight flushes");
			}
		if (p1sf.bIsStraightFlush) { return player_1; }
		if (p2sf.bIsStraightFlush) { return player_2; }
		}

	// Four of a kind (high card wins tie, high set of 4 wins tie of that)
		{
		const auto p14oak = Has4OfAKind (player1);
		const auto p24oak = Has4OfAKind (player2);
		if (p14oak.bHas4oak && p24oak.bHas4oak)
			{
			// FIXME!
			throw std::exception ("Not implemented");
			}
		if (p14oak.bHas4oak) { return player_1; }
		if (p24oak.bHas4oak) { return player_2; }
		}

	// Full House : Three of a kind and a pair. (high three of a kind wins tie)
		{
		const auto p1fh = IsFullHouse (player1);
		const auto p2fh = IsFullHouse (player2);
		if (p1fh.bIsFullHouse && p2fh.bIsFullHouse)
			{
			throw std::exception ("Not implemented");
			}
		if (p1fh.bIsFullHouse) { return player_1; }
		if (p2fh.bIsFullHouse) { return player_2; }		
		}

	// Flush : All cards of the same suit.
		{
		const auto p1flush = IsFlush (player1);
		const auto p2flush = IsFlush (player2);
		if (p1flush.bIsFlush && p2flush.bIsFlush)
			{
			throw std::exception("not implemented");
			}
		if (p1flush.bIsFlush) { return player_1; }
		if (p2flush.bIsFlush) { return player_2; }
		}
		
	// Straight : All cards are consecutive values.
		{
		const auto p1straight = IsStraight (player1);
		const auto p2straight = IsStraight (player2);
		
		if (p1straight.bIsStraight && p2straight.bIsStraight)
			{
			throw std::exception ("Not implemented");
			}
		
		if (p1straight.bIsStraight) { return player_1; }
		if (p2straight.bIsStraight) { return player_2; }
		}

	// Three of a Kind : Three cards of the same value.
		{
		const auto p13oak = Has3OfAKind (player1);
		const auto p23oak = Has3OfAKind (player2);

		if (p13oak.bHas3Oak && p23oak.bHas3Oak)
			{
			throw std::exception ("Not implemented");
			}

		if (p13oak.bHas3Oak) { return player_1; }
		if (p23oak.bHas3Oak) { return player_2; }
		}

	// Two Pairs : Two different pairs.
		{
		const auto p12pair = HasTwoPair (player1);
		const auto p22pair = HasTwoPair (player2);

		if (p12pair.bHasTwoPair && p22pair.bHasTwoPair)
			{
			throw std::exception("Not implemented");
			}

		if (p12pair.bHasTwoPair) { return player_1; }
		if (p22pair.bHasTwoPair) { return player_2; }
		}

	// One Pair : Two cards of the same value.
		{
		const auto p1pair = HasPair (player1);
		const auto p2pair = HasPair (player2);

		if (p1pair.bHasPair && p2pair.bHasPair)
			{
			throw std::exception ("Not implemented");
			}

		if (p1pair.bHasPair) { return player_1; }
		if (p2pair.bHasPair) { return player_2; }
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


CEvaluator::player_t CEvaluator::PlayerWithHighCard (std::vector<value_t> player1, std::vector<value_t> player2)
	{
	if (player1.size () == 0 || player2.size () == 0 || player1.size () != player2.size ())
		{
		throw std::exception ("Unexpected hand size");
		}

	auto p1_sorted = _GetSortedValueSet (player1);
	auto p2_sorted = _GetSortedValueSet (player2);
	}

const std::vector<value_t> CEvaluator::_GetSortedValueSet (const CHand& hand, std::function<bool (value_t)> pred/* = []() { return false; }*/)
	{
	CHand::hand_t copy (hand.get());
	if (copy.size () == 0)
		{
		throw std::exception("Unexpected size");
		}

	std::sort(copy.begin(), copy.end());
	std::vector<value_t> rv;
	std::for_each (copy.begin (), copy.end (), [&rv, &pred](const CCard& c) { if (pred (c.GetValue ())) { rv.push_back (c.GetValue ()); }});
	return rv;
	}
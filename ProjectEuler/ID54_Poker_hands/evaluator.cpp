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

	return { bIsStraightFlush, _GetSortedValueSet(hand).back() };
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
	auto four_oak = _HasXOfAKind (hand, 4);
	return { four_oak.bHas, _GetSortedValueSet (hand, [&four_oak](const value_t v) {return (v != four_oak.xoak);}).back(), four_oak.xoak };
	}

const CEvaluator::full_house_t CEvaluator::IsFullHouse (const CHand& hand)
	{
	auto ThreeOAK = Has3OfAKind(hand);
	auto Pair = HasPair(hand);


	return { ThreeOAK.bHas3Oak && Pair.bHasPair, ThreeOAK.three_cards_value };
	}

const CEvaluator::three_oak_t CEvaluator::Has3OfAKind (const CHand& hand)
	{
	auto ThreeOak = _HasXOfAKind (hand, 3);
	return { ThreeOak.bHas, ThreeOak.xoak, _GetSortedValueSet (hand, [&ThreeOak] (value_t v) { return v != ThreeOak.xoak; }) };
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

	std::vector<value_t> pairs;
	std::for_each (values.begin (), values.end (), [&pairs](const std::pair<value_t, int>& val) {if (val.second == 2) { pairs.push_back(val.first); } });

	if (pairs.size () == 2)
		{
		value_t high_card = _GetSortedValueSet (hand, [&pairs] (value_t v) {return (v != pairs[0] && v != pairs[1]);}).back();
		if (pairs[0] > pairs[1])
			{
			return { pairs.size () == 2, pairs[0], pairs[1], high_card };
			}
		return { pairs.size () == 2, pairs[1], pairs[0], high_card };
		}
	return { pairs.size () == 2, Three, Two, Four };
	}

const CEvaluator::pair_t CEvaluator::HasPair (const CHand& hand)
	{
	auto two_oak = _HasXOfAKind (hand, 2);
	return { two_oak.bHas, two_oak.xoak, _GetSortedValueSet (hand, [&two_oak](value_t v) { return v != two_oak.xoak; }) };
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
			player_t w = _winner(p1sf.high_card, p2sf.high_card);
			if (w == tie)
				{
				throw std::exception("Overall tie");
				}
			return w;
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
			player_t w = _winner (p14oak.four_oak_card, p24oak.four_oak_card);
			if (w == tie)
				{
				w = _winner (p14oak.high_card, p24oak.high_card);
				if (w == tie)
					{
					throw std::exception ("Overall tie");
					}
				}
			return w;
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
			return _winner(p1fh.three_value, p2fh.three_value);
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
			player_t w = _winner (p1flush.sorted_cards, p2flush.sorted_cards);
			if (w == tie)
				{
				throw std::exception ("Overall tie");
				}
			return w;
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
			player_t w = _winner(p1straight.high_card, p2straight.high_card);

			if (w == tie)
				{
				throw std::exception ("Overall tie");
				}

			return w;
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
			player_t w = _winner (p13oak.three_cards_value, p23oak.three_cards_value);
			if (w == tie)
				{
				w = _winner (p13oak.sorted_rem_cards, p23oak.sorted_rem_cards);
				if (w == tie)
					{
					throw std::exception ("Overall tie");
					}
				}
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
			player_t w = _winner (p12pair.high_pair, p22pair.high_pair); 
			if (w == tie)
				{
				w = _winner (p12pair.low_pair, p22pair.low_pair);
				if (w == tie)
					{
					w = _winner (p12pair.high_card, p22pair.high_card);
					if (w == tie)
						{
						throw std::exception ("Overall tie");
						}
					}
				}
			return w;
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
			player_t w = _winner (p1pair.pair_value, p2pair.pair_value);
			if (w = tie)
				{
				w = _winner (p1pair.sorted_rem_cards, p2pair.sorted_rem_cards);
				if (w == tie)
					{
					throw std::exception ("Overall tie");
					}
				}
			return w;
			}

		if (p1pair.bHasPair) { return player_1; }
		if (p2pair.bHasPair) { return player_2; }
		}

	// High Card : Highest value card.
	player_t w = _winner (_GetSortedValueSet(player1), _GetSortedValueSet(player2));
	if (w == tie)
		{
		throw std::exception ("Overall Tie");
		}
	return w;
	}

const CEvaluator::has_xoak CEvaluator::_HasXOfAKind (const CHand& hand, int number)
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
	
	value_t last_match;
	std::for_each (values.begin (), values.end (), [&number, &last_match](const std::pair<value_t, int>& p) {if (p.second == number) { last_match = p.first; }; });

	return has_xoak{ std::any_of (values.begin (), values.end (), [&number](const std::pair<value_t, int>& val) {return val.second == number; }), last_match};
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

CEvaluator::player_t CEvaluator::_winner (value_t player1, value_t player2)
	{
	if (lt (player2, player1))
		{
		return player_1;
		}
	else if (lt (player1, player2))
		{
		return player_2;
		}
	return tie;
	}


CEvaluator::player_t CEvaluator::_winner (std::vector<value_t> player1, std::vector<value_t> player2)
	{
	if (player1.size () == 0 || player2.size () == 0 || player1.size () != player2.size ())
		{
		throw std::exception ("Unexected size");
		}

	player_t w = tie;
	for (int i = player1.size () - 1; i >= 0; --i)
		{
		w = _winner (player1[i], player2[i]);
		if (w != tie)
			{
			break;
			}
		}
	return w;
	}
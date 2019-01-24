// Copyright 2019 Ian Maines
#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>


namespace card
{
enum suit_t
	{
	// Using the letter used to represent the suit in the file
	Clubs = 'C',
	Diamonds = 'D',
	Hearts = 'H',
	Spades = 'S',
	};

enum value_t
	{
	Two = '2',
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten = 'T',
	Jack = 'J',
	Queen = 'Q',
	King = 'K',
	Ace = 'A',
	};

static const std::vector<char> value_order = { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
static bool op_lt(const value_t& rhs, const value_t lhs)	// FIXME NAME
	{
	// Sorted in ascending value. Vector iterators support value comparsons
	// Can't use std::find because it would rely on.. well this operator.
	// Go with O(n) for now. Fairly small number of cards
	auto i_rhs = value_order.begin ();
	while (i_rhs != value_order.end ()) { if (*i_rhs == rhs) { break; } }
	auto i_lhs = value_order.begin ();
	while (i_lhs != value_order.end ()) { if (*i_lhs == lhs) { break; } }

	if (i_rhs == value_order.end () || i_lhs == value_order.end ())
		{
		throw std::exception ("Unexpected value");
		}

	return i_rhs < i_lhs;
	};

static bool lt (value_t lhs, value_t rhs)
	{
	static const std::vector<char> value_order = { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };
	// Return true if we're less than rhs.
	std::vector<char>::const_iterator i_lhs = value_order.begin ();	// Us
	std::vector<char>::const_iterator i_rhs = value_order.begin ();

	// Can't use std::find because list isn't value-sorted and won't yield the result we want.
	while (i_lhs != value_order.end ()) { if (*i_lhs == lhs) { break; } ++i_lhs; }
	while (i_rhs != value_order.end ()) { if (*i_rhs == rhs) { break; } ++i_rhs; }

	return i_lhs < i_rhs;
	}

class CCard
	{
	public:
		CCard (value_t value, suit_t suit)
			: m_value (value)
			, m_suit (suit)
			{}

		std::string print () const
			{
			char str[3];
			sprintf_s (str, "%c%c", m_value, m_suit);
			return std::string (str);
			}

		value_t GetValue () const { return m_value; }
		suit_t GetSuit () const { return m_suit; }

		// Needed to sort and rank cards by face value since we use the underlying value of the enum as the char representation of the card and not its rank.
		// **** NOTE ****: This implementation means that CCard is not suitable for use in data structures
		//				   such as std::set or std::map if the comparitive operators (<,>,==,!=) need to account for both suit and face value.
		//				   Use those containers with card_absolute_compare_t
		bool operator< (const CCard& rhs) const
			{
			return lt (GetValue(), rhs.GetValue ());
			}

		// For consistency since we have operator<
		bool operator== (const CCard& rhs) const
			{
			return (m_value == rhs.m_value) && (m_suit == rhs.m_suit);
			}

		bool operator!= (const CCard& rhs)
			{
			return !(*this == rhs);
			}

		bool operator> (const CCard& rhs)
			{
			return (*this != rhs) && (!(*this < rhs));
			}

	private:
		value_t m_value;
		suit_t m_suit;
	};

struct card_absolute_compare_t
	{
	bool operator() (const CCard& lhs, const CCard& rhs)
		{
		// Cards are first ordered by value, then by suit
		if (rhs.GetValue () == lhs.GetValue ())
			{
			// For this operation, we rank suits in ascending order as Clubs,Diamonds,Hearts,Spades
			static const std::vector<char> suit_value_order = { Clubs, Diamonds, Hearts, Spades };
			std::vector<char>::const_iterator i_lhs = suit_value_order.begin ();	// Us
			std::vector<char>::const_iterator i_rhs = suit_value_order.begin ();

			// Can't use std::find because list isn't value-sorted and won't yield the result we want.
			while (i_lhs != suit_value_order.end ()) { if (*i_lhs == lhs.GetSuit ()) { break; } ++i_lhs; }
			while (i_rhs != suit_value_order.end ()) { if (*i_rhs == rhs.GetSuit ()) { break; } ++i_rhs; }

			return i_lhs < i_rhs;
			}
		return lhs < rhs;
		}
	};

class CHand
	{
	public:
		using hand_t = std::vector<CCard>;

		CHand (const hand_t&& hand)
			: m_hand (hand)
			{
			if (hand.size () != 5)
				{
				throw std::exception ("Expect hand size of 5");
				}
			// Can'd have duplicate cards
			std::set<CCard, card_absolute_compare_t> cards;
			std::for_each (m_hand.begin (), m_hand.end (), [&cards](const CCard& c) { cards.insert(c);});
			if (cards.size () != 5)
				{
				throw std::exception ("Faulty deck: Two identical cards.");
				}
			}

		std::string print () const
			{
			std::stringstream sstr;
			std::for_each (m_hand.begin (), m_hand.end (), [&sstr](const CCard& card)
				{
				sstr << card.print () << " ";
				});
			std::string str (sstr.str ());
			// remove trailing space character
			str.erase (str.end () - 1);
			return str;
			}

		const hand_t& get () const { return m_hand; }

	private:
		hand_t m_hand;
	};
}// namespace card

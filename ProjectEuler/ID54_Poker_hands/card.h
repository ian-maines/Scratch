// Copyright 2019 Ian Maines
#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

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


static const std::vector<value_t> value_order = { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };


static bool operator== (const value_t& rhs, const value_t lhs)
	{
	return static_cast<size_t>(rhs) == static_cast<size_t>(lhs);
	}

static bool operator< (const value_t& rhs, const value_t lhs)
	{
	// Sorted in ascending value. Vector iterators support value comparsons
	auto i_rhs = std::find(value_order.begin(), value_order.end(), rhs);
	auto i_lhs = std::find(value_order.begin (), value_order.end(), lhs);
	if (i_rhs == value_order.end () || i_lhs == value_order.end ())
		{
		throw std::exception ("Unexpected value");
		}

	return i_rhs < i_lhs;
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

	private:
		const value_t m_value;
		const suit_t m_suit;
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

// Copyright 2019 Ian Maines
#pragma once

#include <string>

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
	Ten,
	Jack = 'J',
	Queen = 'Q',
	King = 'K',
	Ace = 'A',
	};

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

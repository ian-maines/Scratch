// ID54_Poker_hands.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

/* https://projecteuler.net/problem=54
In the card game poker, a hand consists of five cards and are ranked, from lowest to highest, in the following way:

High Card: Highest value card.
One Pair: Two cards of the same value.
Two Pairs: Two different pairs.
Three of a Kind: Three cards of the same value.
Straight: All cards are consecutive values.
Flush: All cards of the same suit.
Full House: Three of a kind and a pair.
Four of a Kind: Four cards of the same value.
Straight Flush: All cards are consecutive values of same suit.
Royal Flush: Ten, Jack, Queen, King, Ace, in same suit.
The cards are valued in the order:
2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace.

If two players have the same ranked hands then the rank made up of the highest value wins;
for example, a pair of eights beats a pair of fives (see example 1 below). But if two ranks tie,
for example, both players have a pair of queens, then highest cards in each hand are compared (see example 4 below);
if the highest cards tie then the next highest cards are compared, and so on.

Consider the following five hands dealt to two players:

Hand	 	Player 1	 	Player 2	 		Winner
1	 	5H 5C 6S 7S KD		2C 3S 8S 8D TD		Player 2
		Pair of Fives		Pair of Eights

2	 	5D 8C 9S JS AC		2C 5C 7D 8S QH		Player 1
		Highest card Ace	Highest card Queen

3	 	2D 9C AS AH AC		3D 6D 7D TD QD		Player 2
		Three Aces			Flush with Diamonds

4	 	4D 6S 9H QH QC		3D 6D 7H QD QS		Player 1
		Pair of Queens		Pair of Queens
		Highest card Nine	Highest card Seven

5	 	2H 2D 4C 4D 4S		3C 3D 3S 9S 9D		Player 1
		Full House			Full House
		With Three Fours	with Three Threes
The file, poker.txt, contains one-thousand random hands dealt to two players
. Each line of the file contains ten cards (separated by a single space): the first five are Player 1's cards and the last five are Player 2's cards. 
You can assume that all hands are valid (no invalid characters or repeated cards), each player's hand is in no specific order, and in each hand there is a clear winner.

How many hands does Player 1 win?
*/

// Tasks
// 1. Load file
// 2. Data structure to represent a hand
// 3. logic to compare two hands and select a winner.

namespace
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
				return std::string(str);
				}

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
				{}

			std::string print () const
				{
				std::stringstream sstr;
				std::for_each (m_hand.begin (), m_hand.end (), [&sstr](const CCard& card)
					{
					sstr << card.print() << "";
					});
				std::string str (sstr.str());
				// remove trailing space character
				str.erase(str.end () - 1);
				return str;
				}

		private:
			hand_t m_hand;
		};

	class CMatch
		{
		public:
			CMatch (const CHand& Player1, const CHand& Player2)
				: m_p1 (Player1)
				, m_p2 (Player2)
				{}

			std::string print () const
				{
				std::stringstream ss;
				ss << m_p1.print () << " " << m_p2.print();
				return ss.str();
				}

		private:
			CHand m_p1;
			CHand m_p2;
		};

	using tournament_t = std::vector<CMatch>;
	}

namespace file_reader
	{
	tournament_t read_file (const std::string& fname)
		{
		tournament_t tournament;
		std::ifstream ifs;
		ifs.open (fname);

		if (ifs.rdstate () & std::ifstream::failbit)
			{
			throw std::exception ("Unable to open file!");
			}

		std::string line;
		while (std::getline (ifs, line))
			{
			// Each line consists of 10 cards, the first 5 being player 1, the next 5 being player 2
			// Cards are space-delimited.
			CHand::hand_t all_cards;
			for (int i = 0; i < 10; ++i)
				{
				int index = 3*i;
				all_cards.push_back(CCard(value_t (line[index]), suit_t (line[index + 1])));
				}

			tournament.push_back (CMatch (
										CHand (CHand::hand_t (all_cards.begin (), all_cards.begin () + 5)),
										CHand (CHand::hand_t (all_cards.begin () + 5, all_cards.end ()))
								 ));
			std::cout << tournament.back().print() << std::endl;
			}
		return tournament;
		}
	}

int main()
	{
	tournament_t t;
	try
		{
		t = file_reader::read_file (std::string ("..\\ID54_Poker_hands\\poker.txt"));
		}
	catch (std::exception& e)
		{
		std::cout << "Exception: '" << e.what() << "'" << std::endl;
		}
	catch (...)
		{
		std::cout << "Unexpected exception type" << std::endl;
		}

	return 0;
	}
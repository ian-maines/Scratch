// ID54_Poker_hands.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include "card.h"
#include "evaluator.h"
#include "game.h"
#include "file_reader.h"

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
// 3. logic to compare two hands and select a winner.s

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

	std::map<CEvaluator::player_t, size_t> score;
	score[CEvaluator::player_1] = 0;
	score[CEvaluator::player_2] = 0;

	std::for_each (t.begin (), t.end (), [&score] (const CMatch& m) 
		{
		std::cout << m.print_result () << std::endl;
		score[m.winner()]++;
		});

	std::cout <<std::endl << "Player 1 Score: " << score[CEvaluator::player_1] << std::endl <<
		"Player 2 Score: " << score[CEvaluator::player_2] << std::endl;

	return 0;
	}
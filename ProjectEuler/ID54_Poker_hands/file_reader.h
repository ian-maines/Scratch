// Copyright 2019 Ian Maines
#pragma once

#include <fstream>
#include <iostream>

#include "game.h"

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
			int index = 3 * i;
			all_cards.push_back (CCard (value_t (line[index]), suit_t (line[index + 1])));
			}

		tournament.push_back (CMatch (
			CHand (CHand::hand_t (all_cards.begin (), all_cards.begin () + 5)),
			CHand (CHand::hand_t (all_cards.begin () + 5, all_cards.end ()))
		));
		//std::cout << tournament.back ().print () << std::endl;
		}
	return tournament;
	}

}
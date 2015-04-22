//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     main.cpp for traveling sales person.
// Date:            04/21/2015
//
//************************************************************

// C++ headers
#include <iostream>
#include <chrono>
#include <thread>

#include <fstream>
#include <string>
#include <iterator>


// My headers.
#include "thread_base.h"
#include "point.h"
#include "tour.h"

using std::cout;
using std::endl;

namespace
{
using namespace std;
// Print out program-related data.
void preamble ()
    {
    cout << "***************************************" << endl
         << "* Copyright 2015 Ian Maines           *" << endl
         << "* Traveling sales person C++ solution *" << endl
         << "***************************************" << endl;
    return;
    }

// Takes a line as they appear in the data files (123.45 67.890) and parses
// into two doubles
point_t parse_line (const std::string& strLine)
	{
	cout << "parse_line ()" << endl;
	std::string strTemp (strLine.begin (), strLine.begin () + strLine.find_first_of (" "));
	double x;
	x = stod (strTemp);
	strTemp = std::string (strLine.begin () + strLine.find_first_of (" "), strLine.end ());
	point_t point (x, stod(strTemp));
	cout << "[parse_line () created: " << to_string (point) << endl;
	return point;
	}
}

int main(int argc, char* argv[])
    {
    // Print out our preamble
    preamble ();

	std::string strFilePath ("..\\..\\..\\Datasets\\tsp4.txt");

	std::ifstream cDataFile;
	cDataFile.open (strFilePath);
	
	// First line has max x/y vals, currently don't care about that.
	std::string strLine;
	std::getline (cDataFile, strLine);

	// Create a 'tour' of all points.
	tour_t cStartData;

	// Now populate the structure of points.
	while (std::getline (cDataFile, strLine) &&  !strLine.empty ())
		{
		cStartData.push_front (parse_line (strLine));
		}

	cout << to_string (cStartData) << endl;

	// Exit message
	cout << endl << "Press ENTER to exit...";
	std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
    return 0;
    }


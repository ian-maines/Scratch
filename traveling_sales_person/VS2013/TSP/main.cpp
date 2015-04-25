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
#include <vector>
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

// Trim whitespace from both ends of the string
void trim (std::string& str)
	{
	cout << "[trim ()] strating string:\t'" << str << "'" << endl;
	vector<char> whitespace{ ' ', '\t', '\r', '\n' };
	auto iswhitespace = [&whitespace](char ch) {return (std::find (whitespace.begin (), whitespace.end (), ch) != whitespace.end ()); };
	string::iterator itr = str.begin ();
	// From the beginning.
	while (iswhitespace (*itr))
		{
		str.erase (itr);
		++itr;
		}
	// from the end.
	itr = str.end () - 1;
	while (iswhitespace (*itr))
		{
		str.erase (itr);
		--itr;
		}
	cout << "[trim ()] ending string:\t'" << str << "'" << endl;
	}

// Takes a line as they appear in the data files (123.45 67.890) and parses
// into two doubles
point_t parse_line (std::string strLine)
	{
	// Some of the data has a space at the beginning. (Nasty!)
	trim (strLine);
	const size_t nFirstSpace = strLine.find_first_of (" ");
	point_t point (stod (std::string (strLine.begin (), strLine.begin () + nFirstSpace)), stod (std::string (strLine.begin () + nFirstSpace, strLine.end ())));
	return point;
	}
}

int main(int argc, char* argv[])
    {
    // Print out our preamble
    preamble ();

	std::string strFilePath;

	// Someone gave us a path to a file.
	if (argc == 2)
		{
		strFilePath = argv[1];
		}
	else
		{
		strFilePath = ("..\\..\\..\\Datasets\\tsp100.txt");
		}
	
	std::cout << "File to open: " << strFilePath << std::endl;

	std::ifstream cDataFile;
	cDataFile.open (strFilePath);
	if (!cDataFile.is_open ())
		{
		cout << "Unable to open file" << endl;
		}
	
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


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

#include <iostream>
#include <fstream>
#include <string>


// My headers.
#include "thread_base.h"
#include "point.h"
#include "tour.h"

namespace
{
using namespace std;
void preamble ()
    {
    cout << "***************************************" << endl
         << "* Copyright 2015 Ian Maines           *" << endl
         << "* Traveling sales person C++ solution *" << endl
         << "***************************************" << endl;
    return;
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

	// Now populate the structure of points.



    std::this_thread::sleep_for (std::chrono::milliseconds (5000));
    return 0;
    }


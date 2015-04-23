//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     tour related stuff
// Date:            04/21/2015
//
//************************************************************

#ifndef TSP_TOUR_H
#define TSP_TOUR_H

#include <list>
#include <algorithm>
#include <iostream>

#include "point.h"

using std::cout;
using std::endl;

// Typedef for a tour
typedef std::list<point_t> tour_t;

std::string to_string (tour_t tour)
	{
	std::string str;
	std::for_each (tour.begin (), tour.end (), [&str](const point_t& p) { str += to_string (p) + "\n"; });
	return str;
	}

#endif // TSP_TOUR_H
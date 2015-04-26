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

// Calculates the length of a tour.
double tour_distance (const tour_t& tour)
	{
	double fDistance = 0;
	point_t p (*tour.begin ());
	auto dst = [&fDistance, &p](const point_t& p1) 
		{
		fDistance += point_distance (p, p1);
		p = p1;
		};
	std::for_each (tour.begin (), tour.end (), dst);
	return fDistance;
	}

#endif // TSP_TOUR_H
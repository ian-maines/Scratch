//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     point-related stuff
// Date:            04/21/2015
//
//************************************************************

#ifndef TSP_POINT_H
#define TSP_POINT_H

#include <utility>
#include <complex>
#include <string>

// Typedef for a point
typedef std::pair<double, double> point_t;

inline double square(double d)
	{
	return d*d;
	}

// Slow and should not be used in time-critical code
std::string to_string(point_t p)
	{
	std::stringstream str;
	str << "(" << std::to_string(p.first) << ", " << std::to_string(p.second) << ")";
	return str.str();
	}

// absolute value of distance between points
inline double distance (point_t a, point_t b)
	{
	return std::abs (square (a.first - b.first) + square (a.second - b.second));
	}
#endif // TSP_POINT_H
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
class point_t
	{
	public:
		point_t (double x, double y)
			: m_x (x)
			, m_y (y)
			{}

		inline double x ()
			{
			return m_x;
			}

		inline double y ()
			{
			return m_y;
			}

	private:
		double m_x;
		double m_y;
	};

inline double square(double d)
	{
	return d*d;
	}

// Slow and should not be used in time-critical code
std::string to_string(point_t p)
	{
	std::stringstream str;
	str << "(" << (p.x ()) << ", " << (p.y ()) << ")";
	return str.str();
	}

// absolute value of distance between points
inline double distance (point_t a, point_t b)
	{
	return std::abs (square (a.x () - b.x ()) + square (a.y () - b.y ()));
	}
#endif // TSP_POINT_H
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
#include <math.h>

// Typedef for a point
class point_t
	{
	public:
		point_t (double x, double y)
			: m_x (x)
			, m_y (y)
			{}

		point_t (const point_t& p)
			: m_x (p.m_x)
			, m_y (p.m_y)
			{}

		inline double x () const
			{
			return m_x;
			}

		inline double y () const
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
inline double point_distance (const point_t& a, const point_t& b)
	{
	return std::sqrt (square (a.x () - b.x ()) + square (a.y () - b.y ()));
	}
#endif // TSP_POINT_H
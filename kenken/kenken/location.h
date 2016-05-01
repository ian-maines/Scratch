// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>

// Stored as <row, column>
using pt = std::pair<unsigned char, unsigned char>;

// Defines location on the board. 9x9 is 0-8, a-i.
enum class location : unsigned char
	{
	a0 = 0, a1, a2, a3, a4, a5, a6, a7, a8,
	b0, b1, b2, b3, b4, b5, b6, b7, b8,
	c0, c1, c2, c3, c4, c5, c6, c7, c8,
	d0, d1, d2, d3, d4, d5, d6, d7, d8,
	e0, e1, e2, e3, e4, e5, e6, e7, e8,
	f0, f1, f2, f3, f4, f5, f6, f7, f8,
	g0, g1, g2, g3, g4, g5, g6, g7, g8,
	h0, h1, h2, h3, h4, h5, h6, h7, h8,
	i0, i1, i2, i3, i4, i5, i6, i7, i8,
	};

// Way to reference columns
enum class column : unsigned char
	{
	a = 0, b, c, d, e, f, g, h, i
	};

pt l_to_pt (location l);

struct point
	{
	explicit point (location l)
		: m_pt (l_to_pt (l))
		{}

	unsigned char row () const { return m_pt.first; }
	unsigned char col () const { return m_pt.second; }

	bool operator< (const point& rhs) const
		{
		return true;	// TODO: FIXME
		}

	bool operator== (const point& rhs) const
		{
		return true;	// TODO: FIXME
		}

	pt m_pt;
	};

location p_to_l (point p);

using locations_t = std::vector<point>;
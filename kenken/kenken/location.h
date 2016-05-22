// Kenken supporting types
// Copyright Ian Maines 4/30/16
#pragma once

#include <vector>

// Defines location on the board. 9x9 is 0-8, a-i.
enum class location : unsigned char
	{
	a0 = 0, b0, c0, d0, e0, f0, g0, h0, i0,
	a1, b1, c1, d1, e1, f1, g1, h1, i1,
	a2, b2, c2, d2, e2, f2, g2, h2, i2,
	a3, b3, c3, d3, e3, f3, g3, h3, i3,
	a4, b4, c4, d4, e4, f4, g4, h4, i4,
	a5, b5, c5, d5, e5, f5, g5, h5, i5,
	a6, b6, c6, d6, e6, f6, g6, h6, i6,
	a7, b7, c7, d7, e7, f7, g7, h7, i7,
	a8, b8, c8, d8, e8, f8, g8, h8, i8,
	};

// Way to reference columns
enum class column : unsigned char
	{
	a = 0, b, c, d, e, f, g, h, i
	};

class point;
point l_to_pt (location l);
location pt_to_l (point p);

class point
	{
	public:
		point (unsigned char row, unsigned char col)
			: m_row (row)
			, m_col (col)
			{}

		explicit point (location l)
			{
			point p (l_to_pt (l));
			m_row = p.m_row;
			m_col = p.m_col;
			}

		unsigned char row () const { return m_row; }
		unsigned char col () const { return m_col; }

		bool operator< (const point& rhs) const
			{
			return pt_to_l (*this) < pt_to_l (rhs);
			}

		bool operator> (const point& rhs) const
			{
			return rhs < *this;
			}

		bool operator== (const point& rhs) const
			{
			return pt_to_l (*this) == pt_to_l (rhs);
			}

		bool operator!= (const point& rhs) const
			{
			return !(*this == rhs);
			}

	private:
		unsigned char m_row;
		unsigned char m_col;
	};

location p_to_l (point p);

using locations_t = std::vector<point>;
#include "location.h"

pt l_to_pt (location l)
	{
	const auto val = static_cast<unsigned char>(l);
	// l / 9 is the row, l % 9 is the column
	return std::make_pair ((val / 9), (val % 9));
	}

location p_to_l (point p)
	{
	return static_cast<location> ((p.row () * 9) + (p.col ()));
	}
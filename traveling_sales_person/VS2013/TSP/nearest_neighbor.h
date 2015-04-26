//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     nearest neighbor algorithm.
// Date:            04/25/2015
//
//************************************************************

#include <algorithm>

#include "tour.h"
#include "point.h"

#ifndef TSP_NEAREST_NEIGHBOR_H
#define TSP_NEAREST_NEIGHBOR_H

namespace
{
// find_best returns an iterator to the best insert point in the list.
tour_t::iterator find_best (tour_t& tour, point_t point)
	{
	//cout << "Tour size " << tour.size () << endl;
	// Special case.
	if (tour.size () < 2)
		{
		return ++tour.begin ();
		}
	tour_t::const_iterator cEnd (tour.cend ());
	tour_t::iterator itrBestDistance = tour.begin ();
	double fBestDst = point_distance (*itrBestDistance, *(++(tour_t::const_iterator (itrBestDistance))));
	for (tour_t::iterator itr = ++tour.begin (); itr != cEnd; ++itr)
		{
		// Special case where we can't go past the end...
		tour_t::const_iterator next (itr);
		++next;
		double fDist = point_distance (point, (next == tour.end () ? *tour.begin () : *next));
		//cout << "Distance " << fDist << " Best Distance: " << fBestDst << endl;
		if (fDist < fBestDst)
			{
			fBestDst = fDist;
			itrBestDistance = itr;
			}
		}
	return ++itrBestDistance;
	}
}

tour_t nearest_neighbor (const tour_t& rStart)
	{
	// This doesn't make any sense.
	if (rStart.size () <2) return tour_t ();

	tour_t::const_iterator cEnd (rStart.end ());
	tour_t cResult;
	// Add the first two items. The order of these doesn't really matter.
	tour_t::const_iterator source_itr (rStart.begin ());
	cResult.push_back (*source_itr++);
	cResult.push_back (*source_itr++);
	int loops = 0;
	for (; source_itr != cEnd; ++source_itr)
		{
		//cout << "loop # " << loops++ << endl;
		cResult.insert (find_best (cResult, *source_itr), *source_itr);
		}

	return cResult;
	}

#endif // TSP_NEAREST_NEIGHBOR_H
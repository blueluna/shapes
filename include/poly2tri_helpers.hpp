#ifndef POLY2TRI_HELPERS_HPP
#define POLY2TRI_HELPERS_HPP

#include <vector>
#include "../poly2tri/poly2tri.h"

std::vector<p2t::Point*> CreatePolyLine(const Point<double> *points, uint32_t count)
{
	const Point<double> *p_ptr = points;
	const Point<double> *p_end = p_ptr + count;
	std::vector<p2t::Point*> polyline;
	polyline.reserve(count);

	while (p_ptr < p_end) {
		polyline.push_back(new p2t::Point(p_ptr->x, p_ptr->y));
		p_ptr++;
	}
	return polyline;
}

void DestroyPolyLine(std::vector<p2t::Point*> &polyline)
{
	for (auto point : polyline) {
		delete point;
	}
	polyline.clear();
}

#endif // POLY2TRI_HELPERS_HPP

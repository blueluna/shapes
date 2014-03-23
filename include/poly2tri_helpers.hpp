#ifndef POLY2TRI_HELPERS_HPP
#define POLY2TRI_HELPERS_HPP

#include <vector>
#include "../poly2tri/poly2tri.h"

std::vector<p2t::Point*> CreatePolyLine(const double *x, const double *y, uint32_t v_count)
{
	const double *x_ptr = x;
	const double *y_ptr = y;
	const double *x_end = x + v_count;
	std::vector<p2t::Point*> polyline;
	polyline.reserve(v_count);

	while (x_ptr < x_end) {
		polyline.push_back(new p2t::Point(*x_ptr, *y_ptr));
		x_ptr++;
		y_ptr++;
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

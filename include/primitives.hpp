#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

namespace coldstar {

struct Point
{
	double x;
	double y;

	Point() : x(0.0), y(0.0) {}
};

struct Box
{
	double xmin;
	double ymin;
	double xmax;
	double ymax;

	Box() : xmin(0.0), ymin(0.0), xmax(0.0), ymax(0.0) {}
};

struct Vertex {
	float	x;
	float	y;
	float	z;
	float	s;
	float	t;
};

}

#endif // PRIMITIVES_HPP

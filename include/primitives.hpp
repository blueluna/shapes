#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

namespace coldstar {

struct Point
{
	double x;
	double y;

	Point() : x(0.0), y(0.0) {}
	Point(double x_, double y_) : x(x_), y(y_) {}
};

struct Box
{
	double xmin;
	double ymin;
	double xmax;
	double ymax;

	Box() : xmin(0.0), ymin(0.0), xmax(0.0), ymax(0.0) {}

	double Width() const
	{
		return xmax - xmin;
	}

	double Height() const
	{
		return ymax - ymin;
	}

	Point Center() const
	{
		double xoffset = ((xmax - xmin) / 2.0);
		double yoffset = ((ymax - ymin) / 2.0);
		return Point(xmin + xoffset, ymin + yoffset);
	}

	void Grow(double length)
	{
		xmin -= length;
		xmax += length;
		ymin -= length;
		ymax += length;
	}

	void Grow(double x, double y)
	{
		xmin -= x;
		xmax += x;
		ymin -= y;
		ymax += y;
	}

	void Move(double x, double y)
	{
		xmin += x;
		xmax += x;
		ymin += y;
		ymax += y;
	}
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

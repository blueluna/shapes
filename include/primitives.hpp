#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

namespace coldstar {

template<typename T>
struct Point
{
	T x;
	T y;

	Point()
		: x(static_cast<T>(0))
		, y(static_cast<T>(0))
	{
	}

	Point(T x_, T y_)
		: x(x_)
		, y(y_)
	{
	}

	void Set(T x_, T y_)
	{
		x = x_;
		y = y_;
	}
};

template<typename T>
struct Box
{
	T xmin;
	T ymin;
	T xmax;
	T ymax;

	Box()
		: xmin(static_cast<T>(0))
		, ymin(static_cast<T>(0))
		, xmax(static_cast<T>(0))
		, ymax(static_cast<T>(0))
	{
	}

	T Width() const
	{
		return xmax - xmin;
	}

	T Height() const
	{
		return ymax - ymin;
	}

	Point<T> Center() const
	{
		T xoffset = ((xmax - xmin) / static_cast<T>(2));
		T yoffset = ((ymax - ymin) / static_cast<T>(2));
		return Point<T>(xmin + xoffset, ymin + yoffset);
	}

	void Grow(T delta)
	{
		xmin -= delta;
		xmax += delta;
		ymin -= delta;
		ymax += delta;
	}

	void Grow(T dx, T dy)
	{
		xmin -= dx;
		xmax += dx;
		ymin -= dy;
		ymax += dy;
	}

	void Move(T x, T y)
	{
		xmin += x;
		xmax += x;
		ymin += y;
		ymax += y;
	}
};

}

#endif // PRIMITIVES_HPP

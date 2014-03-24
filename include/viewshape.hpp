#ifndef VIEWSHAPE_HPP
#define VIEWSHAPE_HPP

#include <cstdint>
#include <vector>
#include "primitives.hpp"
#include "shapeobject.hpp"

namespace coldstar {

struct ViewPart
{
	int32_t		type;
	int32_t		offset;
	int32_t		length;

	ViewPart() : type(0), offset(0), length(0) {}
	ViewPart(int32_t t, int32_t o, int32_t l)
		: type(t), offset(o), length(l)
	{}
};

class ViewShape
{
protected:
	int32_t					index;
	int32_t					type;
	int32_t					offset;
	int32_t					length;
	Box<double>				bounds;
	std::vector<ViewPart>	parts;

public:
	ViewShape();
	ViewShape(const ShapeObject& other);

	int32_t					GetIndex() const { return index; }
	void					SetOffset(const int32_t offset);
	int32_t					GetOffset() const;
	Box<double>				GetBounds() const;

	std::vector<ViewPart>	GetParts() const;
};

}

#endif // VIEWSHAPE_HPP

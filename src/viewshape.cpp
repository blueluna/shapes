#include "viewshape.hpp"

namespace coldstar {

ViewShape::ViewShape()
{
}

ViewShape::ViewShape(const ShapeObject& other)
	: index(other.GetIndex())
	, type(other.GetType())
	, offset(0)
	, length(other.GetVertexCount())
	, bounds(other.GetBounds())
{
	std::vector<ShapeObject::Part> other_parts = other.GetParts();
	for (auto part : other_parts) {
		parts.push_back(ViewPart(part.type, part.offset, part.length));
	}
}

void ViewShape::SetOffset(const int32_t o)
{
	offset = o;
}

int32_t ViewShape::GetOffset() const
{
	return offset;
}

Box<double> ViewShape::GetBounds() const
{
	return bounds;
}

std::vector<ViewPart> ViewShape::GetParts() const
{
	return parts;
}

}

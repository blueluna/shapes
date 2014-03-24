#include "shapeobject.hpp"

namespace coldstar {

ShapeObject::ShapeObject()
	: index(-1)
	, type(0)
	, vertexCount(0)
	, vertices(0)
{
}

ShapeObject::ShapeObject(const SHPObject* obj)
	: index(-1)
	, type(0)
	, vertexCount(0)
	, vertices(0)
{
	Assign(obj);
}

ShapeObject::ShapeObject(const ShapeObject &other)
	: index(-1)
	, type(0)
	, vertexCount(0)
	, vertices(0)
{
	Assign(other);
}

ShapeObject::~ShapeObject()
{
	Destroy();
}

ShapeObject& ShapeObject::operator = (const ShapeObject& other)
{
	Assign(other);
	return *this;
}

void ShapeObject::Destroy()
{
	if (vertexCount > 0) {
		delete [] vertices;
		vertices = 0;
		vertexCount = 0;
	}
	type = 0;
	index = -1;
}

void ShapeObject::Assign(const SHPObject* obj)
{
	if (obj != 0) {
		if (index >= 0) {
			Destroy();
		}
		index = obj->nShapeId;
		type = obj->nSHPType;
		vertexCount = obj->nVertices;
		if (vertexCount > 0) {
			vertices = new Point<double> [vertexCount];

			double *xptr = obj->padfX, *yptr = obj->padfY;
			Point<double> *pptr = vertices;
			Point<double> *pend = pptr + vertexCount;
			while (pptr < pend) {
				pptr->Set(*xptr, *yptr);
				xptr++;
				yptr++;
				pptr++;
			}
		}
		bounds.ymin = obj->dfYMin;
		bounds.xmin = obj->dfXMin;
		bounds.ymax = obj->dfYMax;
		bounds.xmax = obj->dfXMax;

		int *ps = obj->panPartStart;
		int *pt = obj->panPartType;
		Part part;
		parts.clear();
		for (int n = 0; n < obj->nParts; n++) {
			part.type = *pt;
			part.offset = *ps;
			if ((n+1) == obj->nParts) {
				part.length = vertexCount - *ps;
			}
			else {
				part.length = *(ps+1) - *ps;
			}
			parts.push_back(part);
			ps++;
			pt++;
		}
	}
}

void ShapeObject::Assign(const ShapeObject& other)
{
	if (&other != this) {
		if (index >= 0) {
			Destroy();
		}
		index = other.index;
		type = other.type;
		vertexCount = other.vertexCount;
		if (vertexCount > 0) {
			vertices = new Point<double> [vertexCount];
			memcpy(vertices, other.vertices, other.vertexCount * sizeof(Point<double>));
		}
		bounds = other.bounds;
		parts = other.parts;
	}
}

bool ShapeObject::IsValid() const
{
	return index >= 0;
}

int ShapeObject::GetIndex() const
{
	return index;
}

int ShapeObject::GetType() const
{
	return type;
}

std::string ShapeObject::GetTypeString() const
{
	return ShapeTypeAsString(type);
}

int ShapeObject::GetVertexCount() const
{
	return vertexCount;
}

const Point<double>* ShapeObject::GetVertices() const
{
	return const_cast<const Point<double>*>(vertices);
}

Box<double> ShapeObject::GetBounds() const
{
	return bounds;
}

std::vector<ShapeObject::Part> ShapeObject::GetParts() const
{
	return parts;
}

std::string ShapeTypeAsString(int shapeType)
{
	std::string shape;
	switch (shapeType) {
	case SHPT_NULL:
		shape = "None";
		break;
	case SHPT_POINT:
		shape = "2D Point";
		break;
	case SHPT_ARC:
		shape = "2D Arc";
		break;
	case SHPT_POLYGON:
		shape = "2D Polygon";
		break;
	case SHPT_MULTIPOINT:
		shape = "2D Multi-point";
		break;
	case SHPT_POINTZ:
		shape = "3D Point";
		break;
	case SHPT_ARCZ:
		shape = "3D Arc";
		break;
	case SHPT_POLYGONZ:
		shape = "3D Polygon";
		break;
	case SHPT_MULTIPOINTZ:
		shape = "3D Multi-point";
		break;
	case SHPT_POINTM:
		shape = "2D Measure Point";
		break;
	case SHPT_ARCM:
		shape = "2D Measure Arc";
		break;
	case SHPT_POLYGONM:
		shape = "2D Measure Polygon";
		break;
	case SHPT_MULTIPOINTM:
		shape = "2D Measure Multi-point";
		break;
	case SHPT_MULTIPATCH:
		shape = "Multi-patch";
		break;
	default:
		shape = "Unknown";
	}
	return shape;
}

}

#include "shapeobject.h"

ShapeObject::ShapeObject()
    : object(0)
{
}

ShapeObject::ShapeObject(SHPObject* obj)
    : object(obj)
{
}

ShapeObject::~ShapeObject()
{
    SHPDestroyObject(object);
    object = 0;
}

bool ShapeObject::IsValid()
{
    return object != 0;
}

int ShapeObject::GetType()
{
    return object->nSHPType;
}

std::string ShapeObject::GetTypeString()
{
    return ShapeTypeAsString(GetType());
}

int ShapeObject::GetVertexCount()
{
    return object->nVertices;
}

double* ShapeObject::GetXs()
{
    return object->padfX;
}

double* ShapeObject::GetYs()
{
    return object->padfY;
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

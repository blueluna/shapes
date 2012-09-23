#include "shapeobject.h"

ShapeObject::ShapeObject()
    : index(-1)
    , type(0)
    , vertexCount(0)
    , xVertices(0)
    , yVertices(0)
{
}

ShapeObject::ShapeObject(const SHPObject* obj)
    : index(-1)
    , type(0)
    , vertexCount(0)
    , xVertices(0)
    , yVertices(0)
{
    Assign(obj);
}

ShapeObject::ShapeObject(const ShapeObject &other)
    : index(-1)
    , type(0)
    , vertexCount(0)
    , xVertices(0)
    , yVertices(0)
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
        delete [] xVertices;
        delete [] yVertices;
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
            xVertices = new double [vertexCount];
            yVertices = new double [vertexCount];
            memcpy(xVertices, obj->padfX, vertexCount * sizeof(double));
            memcpy(yVertices, obj->padfY, vertexCount * sizeof(double));
        }
        bounds.ymin = obj->dfYMin;
        bounds.xmin = obj->dfXMin;
        bounds.ymax = obj->dfYMax;
        bounds.xmax = obj->dfXMax;
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
            xVertices = new double [vertexCount];
            yVertices = new double [vertexCount];
            memcpy(xVertices, other.xVertices, vertexCount * sizeof(double));
            memcpy(yVertices, other.yVertices, vertexCount * sizeof(double));
        }
        bounds = other.bounds;
    }
}

bool ShapeObject::IsValid()
{
    return index >= 0;
}

int ShapeObject::GetIndex()
{
    return index;
}

int ShapeObject::GetType()
{
    return type;
}

std::string ShapeObject::GetTypeString()
{
    return ShapeTypeAsString(type);
}

int ShapeObject::GetVertexCount()
{
    return vertexCount;
}

double* ShapeObject::GetXs()
{
    return xVertices;
}

double* ShapeObject::GetYs()
{
    return yVertices;
}

ShapeObject::Box ShapeObject::GetBounds()
{
    return bounds;
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

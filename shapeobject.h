#ifndef SHAPEOBJECT_H
#define SHAPEOBJECT_H

#include <string>

#include <shapefil.h>

class ShapeObject
{
public:
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

protected:
    int         index;
    int         type;
    int         vertexCount;
    double      *xVertices;
    double      *yVertices;
    Box         bounds;

public:
    ShapeObject();
    ShapeObject(const SHPObject* obj);
    ShapeObject(const ShapeObject& other);
    ~ShapeObject();

    ShapeObject& operator = (const ShapeObject& other);

    void Assign(const SHPObject* obj);

    bool IsValid();

protected:
    void Destroy();
    void Assign(const ShapeObject& obj);

public:
    int         GetIndex();
    int         GetType();
    std::string GetTypeString();

    int         GetVertexCount();
    double*     GetXs();
    double*     GetYs();
    Box         GetBounds();
};

std::string ShapeTypeAsString(int shapetype);

#endif // SHAPEOBJECT_H

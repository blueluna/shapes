#ifndef SHAPEOBJECT_H
#define SHAPEOBJECT_H

#include <string>

#include <shapefil.h>

class ShapeObject
{
protected:
    SHPObject* object;

public:
    ShapeObject();
    ShapeObject(SHPObject* obj);
    ~ShapeObject();

    bool IsValid();

protected:
    void Copy(SHPObject* obj);

public:
    int         GetType();
    std::string GetTypeString();

    int         GetVertexCount();
    double*     GetXs();
    double*     GetYs();

friend class ShapeFile;
};

std::string ShapeTypeAsString(int shapetype);

#endif // SHAPEOBJECT_H

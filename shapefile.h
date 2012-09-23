#ifndef SHAPEFILE_H
#define SHAPEFILE_H

#include <string>
#include <shapefil.h>

#include "shapeobject.h"

class ShapeFile
{
public:
    class Bounds {
    public:
        int x;
        int y;
        int z;
        int m;
    };

protected:
    SHPHandle   handle;
    int         entityCount;
    int         shapeType;

public:
    ShapeFile();
    ~ShapeFile();

    int     Open(std::string filepath);
    void    Close();

    int         GetEntityCount() { return entityCount; }
    int         GetType() { return shapeType; }
    std::string GetTypeString();

    int         GetShape(int index, ShapeObject &object);
};

class ShapeFileError
{
};

#endif // SHAPEFILE_H

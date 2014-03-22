#include "shapefile.hpp"

namespace coldstar {

ShapeFile::ShapeFile()
	: handle(0)
	, entityCount(0)
	, shapeType(0)
{
}

ShapeFile::~ShapeFile()
{
	Close();
}

int ShapeFile::Open(std::string filepath)
{
	Close();
	handle = SHPOpen(filepath.c_str(), "rb");
	if (handle == 0) {
		return -1;
	}
	else {
		SHPGetInfo(handle, &entityCount, &shapeType, 0, 0);
	}
	return 0;
}

void ShapeFile::Close()
{
	if (handle != 0) {
		SHPClose(handle);
	}
	handle = 0;
}

std::string ShapeFile::GetTypeString()
{
	return ShapeTypeAsString(shapeType);
}

int ShapeFile::GetShape(int index, ShapeObject &object)
{
	if (handle == 0) {
		return -1;
	}
	SHPObject *obj = SHPReadObject(handle, index);
	object.Assign(obj);
	SHPDestroyObject(obj);
	return 0;
}

}

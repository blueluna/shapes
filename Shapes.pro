#-------------------------------------------------
#
# Project created by QtCreator 2012-09-23T11:38:32
#
#-------------------------------------------------

QT       += core gui opengl network

TARGET = Shapes
TEMPLATE = app

SOURCES += src/shapeobject.cpp \
	src/shapefile.cpp \
	src/openglwidget.cpp \
	src/mainwindow.cpp \
	src/main.cpp \
	src/viewshape.cpp \
	poly2tri/common/shapes.cc \
	poly2tri/sweep/advancing_front.cc \
	poly2tri/sweep/cdt.cc \
	poly2tri/sweep/sweep.cc \
	poly2tri/sweep/sweep_context.cc

HEADERS  += \
	include/primitives.hpp \
	include/viewshape.hpp \
	include/shapeobject.hpp \
	include/shapefile.hpp \
	include/openglwidget.hpp \
	include/mainwindow.hpp \
	poly2tri/poly2tri.h \
	poly2tri/common/shapes.h \
	poly2tri/common/utils.h \
	poly2tri/sweep/advancing_front.h \
	poly2tri/sweep/cdt.h \
	poly2tri/sweep/sweep.h \
	poly2tri/sweep/sweep_context.h \
    include/poly2tri_helpers.hpp

FORMS += ui/mainwindow.ui

RESOURCES += \
	resources.qrc

OTHER_FILES += \
	geometry.glsl \
	fragment.glsl \
	vertex.glsl \
	stylesheet.qss

INCLUDEPATH = include glew/include shapelib
win32:LIBS += -L"$$_PRO_FILE_PWD_/glew/lib" -L"$$_PRO_FILE_PWD_/shapelib" "glew32.lib" "shapelib.lib"

MY_LIB_FILES += "$$_PRO_FILE_PWD_/shapelib/shapelib.dll"
MY_LIB_FILES += "$$_PRO_FILE_PWD_/glew/bin/Release/x64/glew32.dll"

extra_libs.files = MY_LIB_FILES
extra_libs.path = $$OUT_PWD

INSTALLS += extra_libs

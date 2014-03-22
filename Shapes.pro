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
    src/viewshape.cpp

HEADERS  += \
    include/primitives.hpp \
    include/viewshape.hpp \
    include/shapeobject.hpp \
    include/shapefile.hpp \
    include/openglwidget.hpp \
    include/mainwindow.hpp

FORMS    += ui/mainwindow.ui

win32:LIBS += -L"$$_PRO_FILE_PWD_/glew/lib" -L"$$_PRO_FILE_PWD_/shapelib" "glew32.lib" "shapelib.lib"

INCLUDEPATH = include glew/include shapelib

RESOURCES +=

OTHER_FILES +=

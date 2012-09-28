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
    src/main.cpp

HEADERS  += include/shapeobject.h \
    include/shapefile.h \
    include/openglwidget.h \
    include/mainwindow.h

FORMS    += ui/mainwindow.ui

LIBS += -Lglew/lib -lglew32 -Lshapelib -lshapelib

INCLUDEPATH = include glew/include shapelib

RESOURCES +=

OTHER_FILES +=

#-------------------------------------------------
#
# Project created by QtCreator 2012-09-23T11:38:32
#
#-------------------------------------------------

QT       += core gui opengl network

TARGET = Shapes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    shapefile.cpp \
    shapeobject.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    shapefile.h \
    shapeobject.h

FORMS    += mainwindow.ui

LIBS += -Lglew/lib -lglew32 -Lshapelib -lshapelib

INCLUDEPATH = glew/include shapelib

RESOURCES +=

OTHER_FILES +=

#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <gl/glew.h>
#include <QGLWidget>

#include "shapeobject.h"

class OpenGLWidget
    : public QGLWidget
{
    Q_OBJECT

protected:
    int             textureMax;
    int             vertexMax;
    int             indexMax;
    ShapeObject     object;

public:
    explicit OpenGLWidget(QWidget *parent = 0);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();

    void    SetShape(ShapeObject object);

signals:
    
public slots:
    
};

#endif // OPENGLWIDGET_H

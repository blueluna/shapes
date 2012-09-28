#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <gl/glew.h>
#include <QGLWidget>
#include <QGLShaderProgram>

#include "shapeobject.h"

class OpenGLWidget
    : public QGLWidget
{
    Q_OBJECT

public:
    typedef struct vertext_s {
        float	x;
        float	y;
        float	z;
        float	s;
        float	t;
    } vertex_t;

protected:
    int             textureMax;
    int             vertexMax;
    int             indexMax;
    ShapeObject     object;
    bool            objectUpdated;
    QString         objectText;
    ShapeObject::Box    bounds;

    vertex_t        *vertices;
    unsigned int    vertexCount;

    bool                useShanders;
    QGLShaderProgram    *shaderProgram;
    int                 shaderMatrix;
    int                 shaderVertices;
    int                 shaderColor;

public:
    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

    void    SetShape(const ShapeObject &shape);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();

    void        BuildObject();

signals:
    
public slots:
    
};

#endif // OPENGLWIDGET_H

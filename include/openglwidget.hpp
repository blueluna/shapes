#ifndef OPENGLWIDGET_HPP
#define OPENGLWIDGET_HPP

#include <gl/glew.h>
#include <QGLWidget>
#include <QGLShaderProgram>

#include <cstdint>

#include "shapeobject.hpp"
#include "viewshape.hpp"

using namespace coldstar;

class OpenGLWidget
	: public QGLWidget
{
	Q_OBJECT

protected:
	int							textureMax;
	int							vertexMax;
	int							indexMax;

	Vertex						*vertices;
	uint32_t					vertexCount;
	uint32_t					vertexAllocated;

	bool						useShanders;
	QGLShaderProgram			*shaderProgram;
	int							shaderMatrix;
	int							shaderVertices;
	int							shaderColor;

	Box							bounds;
	bool						objectsUpdated;
	std::vector<ViewShape>		objects;

public:
	explicit OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();

	void	AddShape(const ShapeObject &shape);

protected:
	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	void	BuildObject(const ShapeObject& shape);

signals:
	
public slots:
	
};

#endif // OPENGLWIDGET_HPP

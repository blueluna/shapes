#include <vector>

#include "openglwidget.hpp"

#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QGLWidget(parent)
	, objectsUpdated(false)
	, vertices(0)
	, vertexCount(0)
	, vertexAllocated(0)
	, useShanders(false)
	, shaderProgram(0)
	, shaderMatrix(-1)
	, shaderVertices(-1)
	, shaderColor(-1)
{
}

OpenGLWidget::~OpenGLWidget()
{
	if (shaderProgram != 0) {
		delete shaderProgram;
	}
}

void OpenGLWidget::initializeGL()
{
	qglClearColor(QColor(47, 47, 47));
	glClear(GL_COLOR_BUFFER_BIT);

	if (QGLShaderProgram::hasOpenGLShaderPrograms()) {
		useShanders = true;
		qDebug() << "Has shaders.";
		shaderProgram = new QGLShaderProgram(this);
		bool ok = shaderProgram->addShaderFromSourceCode(QGLShader::Vertex,
			"attribute highp vec4 vertex;\n"
			"uniform highp mat4 matrix;\n"
			"void main(void)\n"
			"{\n"
			"   gl_Position = matrix * vertex;\n"
			"}"
		);
		if (!ok) {
			qDebug() << "Failed to add vertex shader.";
		}
		ok = shaderProgram->addShaderFromSourceCode(QGLShader::Fragment,
			 "uniform mediump vec4 color;\n"
			 "void main(void)\n"
			 "{\n"
			 "   gl_FragColor = color;\n"
			 "}");
		if (!ok) {
			qDebug() << "Failed to add fragment shader.";
		}
		if (!shaderProgram->link()) {
			qDebug() << "Failed to link shaders.";
		}
		if (!shaderProgram->bind()) {
			qDebug() << "Failed to bind shaders.";
		}
		shaderVertices = shaderProgram->attributeLocation("vertex");
		shaderMatrix = shaderProgram->uniformLocation("matrix");
		shaderColor = shaderProgram->uniformLocation("color");

		QColor color(255, 255, 255, 255);
		shaderProgram->setUniformValue(shaderColor, color);
	}
	else {
		if (glewIsSupported("GL_ARB_pixel_buffer_object GL_EXT_framebuffer_object")) {
			/*
			TODO: Write code for using ARB_pixel_buffer_object to transfer texture data
			*/
			glShadeModel(GL_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else {
			glShadeModel(GL_FLAT);
		}
		glLineWidth(1.0f);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureMax);
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &vertexMax);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &indexMax);
	qDebug() << "OpenGL limits: Max Texture Size: " << textureMax << " Max Vertex Count: "
			 << vertexMax << " Max Index Count: " << indexMax;
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	double xmin = -180.0, xmax = 180.0, ymin = -90.0, ymax = 90.0;
	xmin = bounds.xmin;
	xmax = bounds.xmax;
	ymin = bounds.ymin;
	ymax = bounds.ymax;

	double xoffset = ((xmax - xmin) / 2.0);
	double yoffset = ((ymax - ymin) / 2.0);
	double xcenter = xmin + xoffset;
	double ycenter = ymin + yoffset;

	float fxmin, fxmax, fymin, fymax;

	double s = (double)(width) / height;
	if (s > 1.0) {
		xoffset = (xoffset * s) + 0.001;
		yoffset = yoffset + 0.001;
	}
	else {
		xoffset = xoffset + 0.001;
		yoffset = (yoffset / s) + 0.001;
	}
	fxmin = xcenter - xoffset;
	fxmax = xcenter + xoffset;
	fymin = ycenter - yoffset;
	fymax = ycenter + yoffset;

	if (useShanders) {
		QMatrix4x4 pmvMatrix;
		pmvMatrix.ortho(fxmin, fxmax, fymin, fymax, 0.0f, 1.0f);
		shaderProgram->setUniformValue(shaderMatrix, pmvMatrix);
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(fxmin, fxmax, fymin, fymax, 0.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (vertexCount <= 0) {
		return;
	}

	if (useShanders) {
		shaderProgram->enableAttributeArray(shaderVertices);
	}
	else {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
		float *fptr = (float*)(vertices);
		glVertexPointer(3, GL_FLOAT, 20, fptr);
		glTexCoordPointer(2, GL_FLOAT, 20, fptr + 3);
	}

	for (auto shape : objects) {
		int32_t shape_offset = shape.GetOffset();
		for (auto part : shape.GetParts()) {
			if (part.type == SHPT_POLYGON) {
				glDrawArrays(GL_LINE_LOOP, part.offset + shape_offset, part.length);
			}
			else {
				glDrawArrays(GL_POINTS, part.offset + shape_offset, part.length);
			}
		}
	}

	if (useShanders) {
		shaderProgram->disableAttributeArray(shaderVertices);
	}
	else {
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void OpenGLWidget::AddShape(const ShapeObject& shape)
{
	objects.push_back(ViewShape(shape));
	BuildObject(shape);
	resizeGL(width(), height());
	paintGL();
}

void OpenGLWidget::BuildObject(const ShapeObject& shape)
{
	int count = shape.GetVertexCount();

	if ((vertexCount + count) > vertexAllocated) {
		uint32_t new_length = (((vertexCount + count) / 10240) + 1) * 10240;
		Vertex *vp = new Vertex [new_length];
		if (vertexCount != 0) {
			memcpy(vp, vertices, vertexCount * sizeof(Vertex));
		}
		delete [] vertices;
		vertices = vp;
		vertexAllocated = new_length;
	}

	objects.back().SetOffset(vertexCount);

	Vertex *vptr = vertices + vertexCount;
	double *xptr = shape.GetXs();
	double *yptr = shape.GetYs();

	for (int i = 0; i < count; i++) {
		vptr->x = static_cast<float>(*xptr);
		vptr->y = static_cast<float>(*yptr);
		vptr->z = 0.0f;
		vptr->s = 0.0f;
		vptr->t = 0.0f;
		vptr++;
		xptr++;
		yptr++;
	}

	vertexCount += count;

	if (useShanders) {
		shaderProgram->setAttributeArray(shaderVertices, (float*)(vertices), 3, 20);
	}

	Box shape_bounds = shape.GetBounds();
	bounds.xmin = shape_bounds.xmin < bounds.xmin ? shape_bounds.xmin : bounds.xmin;
	bounds.xmax = shape_bounds.xmax > bounds.xmax ? shape_bounds.xmax : bounds.xmax;
	bounds.ymin = shape_bounds.ymin < bounds.ymin ? shape_bounds.ymin : bounds.ymin;
	bounds.ymax = shape_bounds.ymax > bounds.ymax ? shape_bounds.ymax : bounds.ymax;
}

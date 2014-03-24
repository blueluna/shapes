#include <vector>

#include "openglwidget.hpp"

#include <QtWidgets>
#include <QtOpenGL>
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
	, polygon(0)
	, polygonVertexCount(0)
{
	setFormat(QGLFormat(QGL::SampleBuffers));
}

OpenGLWidget::~OpenGLWidget()
{
	if (shaderProgram != 0) {
		delete shaderProgram;
	}
	if (vertices != 0) {
		delete [] vertices;
		vertexCount = 0;
		vertexAllocated = 0;
	}
	if (polygon != 0) {
		delete [] polygon;
		polygonVertexCount = 0;
	}
}

void OpenGLWidget::initializeGL()
{
	qglClearColor(QColor(47, 47, 47));
	glClear(GL_COLOR_BUFFER_BIT);
	bool ok;

	if (QGLShaderProgram::hasOpenGLShaderPrograms()) {
		qDebug() << "Has shaders.";
		useShanders = true;
	}
	// useShanders = false;

	if (useShanders) {
		shaderProgram = new QGLShaderProgram(this);
		ok = shaderProgram->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/vertex");
		if (!ok) {
			qDebug() << "Failed to add vertex shader.";
		}
		/*
		ok = shaderProgram->addShaderFromSourceFile(QGLShader::Geometry, ":/shaders/geometry");
		if (!ok) {
			qDebug() << "Failed to add geometry shader.";
		}
		*/
		ok = shaderProgram->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/fragment");
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
	qDebug() << "OpenGL limits: Max Texture Size: " << textureMax
			 << " Max Vertex Count: " << vertexMax
			 << " Max Index Count: " << indexMax;
}

void OpenGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	UpdateViewMatrix();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

Point<double> OpenGLWidget::translateCoordinate(const int x, const int y) const
{
	double xf = x * viewBox.Width() / width();
	double yf = y * viewBox.Height() / height();
	return Point<double>(xf, yf);
}

Point<double> OpenGLWidget::translateCoordinate(const QPoint &p) const
{
	return translateCoordinate(p.x(), p.y());
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();
	Point<double> pt = translateCoordinate(dx, dy);

	if (event->buttons() & Qt::LeftButton) {
		viewBox.Move(-pt.x, pt.y);
	}
	lastPos = event->pos();
	UpdateViewMatrix();
	updateGL();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
	double numDegrees = -event->delta() / 8.0;
	double numSteps = numDegrees / 15.0;
	double mouse_xd = event->x() / static_cast<double>(width());
	double mouse_yd = event->y() / static_cast<double>(height());

	if (event->orientation() == Qt::Vertical) {
		double bounds_width = viewBox.Width();
		double bounds_height = viewBox.Height();
		viewBox.xmin -= numSteps * bounds_width * 0.05 * mouse_xd;
		viewBox.xmax += numSteps * bounds_width * 0.05 * (1.0 - mouse_xd);
		viewBox.ymin -= numSteps * bounds_height * 0.05 * (1.0 - mouse_yd);
		viewBox.ymax += numSteps * bounds_height * 0.05 * mouse_yd;
		UpdateViewMatrix();
		updateGL();
	}
}

void OpenGLWidget::UpdateViewBoxFromBounds()
{
	double xmin = -180.0, xmax = 180.0, ymin = -90.0, ymax = 90.0;
	xmin = bounds.xmin;
	xmax = bounds.xmax;
	ymin = bounds.ymin;
	ymax = bounds.ymax;
	Point<double> center = bounds.Center();
	double xcenter = center.x;
	double ycenter = center.y;
	double xoffset = ((xmax - xmin) / 2.0);
	double yoffset = ((ymax - ymin) / 2.0);

	float fxmin, fxmax, fymin, fymax;

	double ratio = width() / static_cast<double>(height());
	if (ratio > 1.0) {
		xoffset = (xoffset * ratio);
		yoffset = yoffset;
	}
	else {
		xoffset = xoffset;
		yoffset = (yoffset / ratio);
	}
	fxmin = xcenter - xoffset;
	fxmax = xcenter + xoffset;
	fymin = ycenter - yoffset;
	fymax = ycenter + yoffset;

	viewBox.xmin = xcenter - xoffset;
	viewBox.xmax = xcenter + xoffset;
	viewBox.ymin = ycenter - yoffset;
	viewBox.ymax = ycenter + yoffset;
	UpdateViewMatrix();
}

void OpenGLWidget::UpdateViewMatrix()
{
	if (useShanders) {
		QMatrix4x4 pmvMatrix;
		pmvMatrix.ortho(
			static_cast<float>(viewBox.xmin)
			, static_cast<float>(viewBox.xmax)
			, static_cast<float>(viewBox.ymin)
			, static_cast<float>(viewBox.ymax)
			, 0.0f, 1.0f
			);
		shaderProgram->setUniformValue(shaderMatrix, pmvMatrix);
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(
			static_cast<float>(viewBox.xmin)
			, static_cast<float>(viewBox.xmax)
			, static_cast<float>(viewBox.ymin)
			, static_cast<float>(viewBox.ymax)
			, 0.0f, 1.0f);
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

	if (polygonVertexCount > 0) {

		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4f(1.0f, 0.0f, 0.0f, 0.7f);
		float *fptr = (float*)(polygon);
		glVertexPointer(2, GL_FLOAT, 0, fptr);
		if (useShanders) {
			shaderProgram->setUniformValue(shaderColor, QColor(255, 255, 255));
		}
		glDrawArrays(GL_TRIANGLES, 0, polygonVertexCount);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (useShanders) {
			shaderProgram->setUniformValue(shaderColor, QColor(64, 127, 127));
		}
		glDrawArrays(GL_TRIANGLES, 0, polygonVertexCount);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (useShanders) {
		shaderProgram->enableAttributeArray(shaderVertices);
	}
	else {
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
		float *fptr = (float*)(vertices);
		glVertexPointer(2, GL_FLOAT, 0, fptr);
	}

	for (auto shape : objects) {

		if (useShanders) {
			QColor color(255, 255, 255, 255);
			if (shape.GetIndex() == selectedObject) {
				color.setRgb(128, 196, 221);
			}
			shaderProgram->setUniformValue(shaderColor, color);
		}
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
	}
}

void OpenGLWidget::AddShape(const ShapeObject& shape)
{
	objects.push_back(ViewShape(shape));
	BuildObject(shape);
}

void OpenGLWidget::BuildObject(const ShapeObject& shape)
{
	int count = shape.GetVertexCount();

	if ((vertexCount + count) > vertexAllocated) {
		uint32_t new_length = (((vertexCount + count) / 10240) + 1) * 10240;
		Point<float> *vp = new Point<float> [new_length];
		if (vertexCount != 0) {
			memcpy(vp, vertices
				   , vertexCount * sizeof(Point<float>));
		}
		if (vertices != 0) {
			delete [] vertices;
		}
		vertices = vp;
		vertexAllocated = new_length;
	}

	objects.back().SetOffset(vertexCount);

	const Point<double> *src_ptr = shape.GetVertices();
	Point<float> *dst_ptr = vertices + vertexCount;

	for (int i = 0; i < count; i++) {
		dst_ptr->x = static_cast<float>(src_ptr->x);
		dst_ptr->y = static_cast<float>(src_ptr->y);
		src_ptr++;
		dst_ptr++;
	}

	vertexCount += count;

	if (useShanders) {
		shaderProgram->setAttributeArray(shaderVertices, (float*)(vertices), 2, 0);
	}
}

void OpenGLWidget::Zoom(const int32_t shapeIndex)
{
	bounds.xmin = 10000.0;
	bounds.xmax = -10000.0;
	bounds.ymin = 10000.0;
	bounds.ymax = -10000.0;

	selectedObject = shapeIndex;

	if (shapeIndex < 0) {
		for (auto shape : objects) {
			Box<double> shape_bounds = shape.GetBounds();
			bounds.xmin = shape_bounds.xmin < bounds.xmin ? shape_bounds.xmin : bounds.xmin;
			bounds.xmax = shape_bounds.xmax > bounds.xmax ? shape_bounds.xmax : bounds.xmax;
			bounds.ymin = shape_bounds.ymin < bounds.ymin ? shape_bounds.ymin : bounds.ymin;
			bounds.ymax = shape_bounds.ymax > bounds.ymax ? shape_bounds.ymax : bounds.ymax;
		}
	}
	else {
		if (objects.size() > shapeIndex) {
			Box<double> shape_bounds = objects[shapeIndex].GetBounds();
			double width = shape_bounds.Width();
			double height = shape_bounds.Height();
			double side = height > width ? height : width;
			shape_bounds.Grow(side * 2);
			bounds = shape_bounds;
		}
	}
	UpdateViewBoxFromBounds();
}

void OpenGLWidget::SetPolygon(const std::vector<p2t::Triangle*> &poly)
{
	if (polygon != 0) {
		delete [] polygon;
		polygonVertexCount = 0;
	}
	uint32_t len = static_cast<uint32_t>(poly.size() * 3);
	polygon = new Point<float> [len];
	polygonVertexCount = len;

	Point<float> *dst_ptr = polygon;

	p2t::Point* pt = 0;
	for (auto ptri : poly) {
		for (int i = 0; i < 3; i++) {
			pt = ptri->GetPoint(i);
			dst_ptr->x = static_cast<float>(pt->x);
			dst_ptr->y = static_cast<float>(pt->y);
			dst_ptr++;
		}
	}
}

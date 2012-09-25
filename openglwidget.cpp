#include <vector>

#include "openglwidget.h"

#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , objectUpdated(false)
    , vertexCount(0)
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
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    double xmin = -180.0, xmax = 180.0, ymin = -90.0, ymax = 90.0;
    if (object.IsValid()) {
        ShapeObject::Box bounds = object.GetBounds();
        xmin = bounds.xmin;
        xmax = bounds.xmax;
        ymin = bounds.ymin;
        ymax = bounds.ymax;
    }

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

    if (useShanders) {
         // Draw shape
         if (vertexCount > 0) {
             shaderProgram->enableAttributeArray(shaderVertices);

             std::vector<ShapeObject::Part> parts = object.GetParts();
             std::vector<ShapeObject::Part>::const_iterator iter = parts.begin();
             while (iter != parts.end()) {
                 if (iter->type == SHPT_POLYGON) {
                     glDrawArrays(GL_LINE_LOOP, iter->offset, iter->length);
                 }
                 else {
                     glDrawArrays(GL_POINTS, iter->offset, iter->length);
                 }
                 iter++;
             }
             shaderProgram->disableAttributeArray(shaderVertices);
         }
    }
    else {
        // Draw shape
        if (vertexCount > 0) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
            float *fptr = (float*)(vertices);
            glVertexPointer(3, GL_FLOAT, 20, fptr);
            glTexCoordPointer(2, GL_FLOAT, 20, fptr + 3);
            std::vector<ShapeObject::Part> parts = object.GetParts();
            std::vector<ShapeObject::Part>::const_iterator iter = parts.begin();
            while (iter != parts.end()) {
                if (iter->type == SHPT_POLYGON) {
                    glDrawArrays(GL_LINE_LOOP, iter->offset, iter->length);
                }
                else {
                    glDrawArrays(GL_POINTS, iter->offset, iter->length);
                }
                iter++;
            }
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
}

void OpenGLWidget::SetShape(const ShapeObject& shape)
{
    object = shape;
    BuildObject();
    resizeGL(width(), height());
    paintGL();
}

void OpenGLWidget::BuildObject()
{
    int count = object.GetVertexCount();

    if (vertexCount != count) {
        if (vertexCount != 0) {
            delete [] vertices;
        }
        vertices = new vertex_t [count];
        vertexCount = count;
    }

    vertex_t *vptr = vertices;
    double *xptr = object.GetXs();
    double *yptr = object.GetYs();

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

    if (useShanders) {
        shaderProgram->setAttributeArray(shaderVertices, (float*)(vertices), 3, 20);
    }
}

#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void OpenGLWidget::initializeGL()
{
    qglClearColor(QColor(47, 47, 47));
    glClear(GL_COLOR_BUFFER_BIT);
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

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureMax);
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &vertexMax);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &indexMax);
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    float s, xs, ys;
    s = (float)(width) / height;
    if (s > 1.0) {
        xs = s;
        ys = 1.0;
    }
    else {
        xs = 1.0;
        ys = (float)(height) / width;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-xs, xs, ys, -ys, 0.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    QString version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    renderText(0.0, 0.0, 0.0, version);

    if (object.IsValid()) {

    }
}

void OpenGLWidget::SetShape(ShapeObject object)
{

}

#include <vector>

#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , objectUpdated(false)
    , vertexCount(0)
    , indexCount(0)
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(fxmin, fxmax, fymin, fymax, 0.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw shape
    if (indexCount > 0) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
        float *fptr = (float*)(vertices);
        glVertexPointer(3, GL_FLOAT, 20, fptr);
        glTexCoordPointer(2, GL_FLOAT, 20, fptr + 3);
        /*
        Handle driver limited vertex count i.e. opengl software rendering over remote desktop
        */
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

    if (indexCount != count) {
        if (indexCount != 0) {
            delete [] indicies;
        }
        indicies = new unsigned int [count];
        indexCount = count;
    }

    vertex_t *vptr = vertices;
    unsigned int *iptr = indicies;
    double *xptr = object.GetXs();
    double *yptr = object.GetYs();

    unsigned int index = 0;
    for (int i = 0; i < count; i++) {
        vptr->x = static_cast<float>(*xptr);
        vptr->y = static_cast<float>(*yptr);
        vptr->z = 0.0f;
        vptr->s = 0.0f;
        vptr->t = 0.0f;
        *iptr = index;
        vptr++;
        index++;
        iptr++;
        xptr++;
        yptr++;
    }
}

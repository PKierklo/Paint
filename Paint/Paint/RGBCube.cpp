#include "RGBCube.h"
#include <GL/gl.h>
#include <GL/glu.h>

BEGIN_EVENT_TABLE(RGBCube, wxGLCanvas)
EVT_PAINT(RGBCube::OnPaint)
EVT_MOTION(RGBCube::OnMouseMotion)
EVT_LEFT_DOWN(RGBCube::OnMouseDown)
EVT_LEFT_UP(RGBCube::OnMouseUp)
END_EVENT_TABLE()

RGBCube::RGBCube(wxWindow* parent)
    : wxGLCanvas(parent, wxID_ANY, nullptr)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void RGBCube::OnPaint(wxPaintEvent& event) {
    wxGLContext context(this);
    SetCurrent(context);

    Render();

    SwapBuffers();
}

void RGBCube::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -6.0f);

    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);

    // Przód
    SetVertexColor(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    SetVertexColor(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    SetVertexColor(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    SetVertexColor(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Tył
    SetVertexColor(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    SetVertexColor(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    SetVertexColor(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    SetVertexColor(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Góra
    SetVertexColor(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    SetVertexColor(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    SetVertexColor(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    SetVertexColor(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Dół
    SetVertexColor(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    SetVertexColor(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    SetVertexColor(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    SetVertexColor(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Prawo
    SetVertexColor(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    SetVertexColor(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    SetVertexColor(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    SetVertexColor(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Lewo
    SetVertexColor(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    SetVertexColor(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    SetVertexColor(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    SetVertexColor(0.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glEnd();

    glFlush();
}

void RGBCube::OnMouseMotion(wxMouseEvent& event) {
    if (dragging) {
        wxPoint currentPos = event.GetPosition();
        int dx = currentPos.x - lastMousePos.x;
        int dy = currentPos.y - lastMousePos.y;

        angleX += dy * 0.5f;
        angleY += dx * 0.5f;

        lastMousePos = currentPos;

        Refresh();
    }
}

void RGBCube::OnMouseDown(wxMouseEvent& event) {
    dragging = true;
    lastMousePos = event.GetPosition();
}

void RGBCube::OnMouseUp(wxMouseEvent& event) {
    dragging = false;
}

void RGBCube::RotateCube(float xAngle, float yAngle) {
    angleX += xAngle;
    angleY += yAngle;
    Refresh();
}

void RGBCube::SetVertexColor(float x, float y, float z) {
    glColor3f(x, y, z);
}

#ifndef RGB_CUBE_H
#define RGB_CUBE_H

#include <wx/glcanvas.h>

class RGBCube : public wxGLCanvas {
public:
    RGBCube(wxWindow* parent);

    void RotateCube(float xAngle, float yAngle);

private:
    float angleX = 0.0f;
    float angleY = 0.0f;

    void OnPaint(wxPaintEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void Render();
    void SetVertexColor(float x, float y, float z);

    bool dragging = false; 
    wxPoint lastMousePos; 

    DECLARE_EVENT_TABLE()
};

#endif

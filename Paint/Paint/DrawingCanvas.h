#ifndef DRAWING_CANVAS_H
#define DRAWING_CANVAS_H

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <vector>
#include "IDrawingModel.h"

wxPoint CalculateCenter(const std::vector<wxPoint>& points);
wxPoint RotatePoint(const wxPoint& point, const wxPoint& center, double angle);

class DrawingCanvas : public wxPanel
{
public:
    DrawingCanvas(wxFrame* parent, IDrawingModel* model);

    void StartDrawing();
    void StartDrawingTriangle();
    void StartDrawingSquare();
    void StartDrawingStraightLine();
    void StartDrawingOval();
    void StartWritingText();
    void SetCurrentColor(const wxColour& color);
    void DrawToDC(wxDC& dc);
    void ResetFlags();
    void StartSelectingShapes();
    void HighlightSelectedShape(wxDC& dc);
    void MoveSelectedShape(const wxPoint& delta);
    void ShowMoveDialog();
    void OnContextMenu(wxCommandEvent& event);
    void ShowContextMenu(const wxPoint& position);
    void ShowRotateDialog();
    void ShowScaleDialog();
    void StartDrawingBezier();
    void StartEditingBezier();               
    void UpdateBezierControlPoint(const wxPoint& point);
    void StopEditingBezier(); 

    void UpdateRotation(const wxPoint& mousePosition);
    void DrawRotationHandle(wxDC& dc);

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnRightMouseDown(wxMouseEvent& event);
    void DrawScalingHandle(wxDC& dc);
    void UpdateScaling(const wxPoint& mousePosition);

    IDrawingModel* model;
    wxColour currentColor = *wxBLACK;
    bool isDrawing = false;
    bool isDrawingTriangle = false;
    bool isDrawingSquare = false;
    bool isDrawingStraightLine = false;
    bool isDrawingOval = false;
    bool isWritingText = false;

    bool isSelectingShapes = false;
    bool isMovingShape = false;   
    wxPoint initialClickPosition;  
    SelectedShape selectedShape;

    bool isRotating = false;        
    wxPoint rotationHandlePosition;   
    double currentRotationAngle = 0.0;

    bool isScaling = false;            
    wxPoint scalingHandlePosition;       
    wxPoint initialScalingMousePosition;

    bool isDrawingBezier = false;
    std::vector<wxPoint> bezierPoints; 
    bool isEditingBezier = false;                
    size_t editingBezierIndex = -1;              
    int editingControlPointIndex = -1;    

};
enum {
    ID_MOVE_OPTION = 1001,
    ID_ROTATE_OPTION = 1002,
    ID_SCALE_OPTION = 1003
};

#endif

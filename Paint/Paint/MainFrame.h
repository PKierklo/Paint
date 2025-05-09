#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include "DrawingCanvas.h"
#include "IDrawingModel.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(IDrawingModel* model);

private:
    void OnSaveAsPNG(wxCommandEvent& event);
    void OnStartDrawing(wxCommandEvent& event);
    void OnStartDrawingTriangle(wxCommandEvent& event);
    void OnStartDrawingSquare(wxCommandEvent& event);
    void OnStartDrawingStraightLine(wxCommandEvent& event);
    void OnStartDrawingOval(wxCommandEvent& event);
    void OnStartWritingText(wxCommandEvent& event);
    void OnChooseColor(wxCommandEvent& event);
    void OnShowRGBCube(wxCommandEvent& event);
    void OnSelectShape(wxCommandEvent& event);
    void OnStartDrawingBezier(wxCommandEvent& event);
    void OnStartEditingBezier(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnLoad(wxCommandEvent& event);

    int bezierDegree = 0;
    DrawingCanvas* canvas;
    IDrawingModel* model;
};

#endif

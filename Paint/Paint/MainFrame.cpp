#include "MainFrame.h"
#include "ColorPickerDialog.h"
#include "RGBCube.h"
MainFrame::MainFrame(IDrawingModel* model)
    : wxFrame(nullptr, wxID_ANY, "Drawing Application", wxDefaultPosition, wxSize(800, 600)),
    model(model)
{
    canvas = new DrawingCanvas(this, model);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* colorButton = new wxButton(this, wxID_ANY, "Choose Color");
    colorButton->Bind(wxEVT_BUTTON, &MainFrame::OnChooseColor, this);
    buttonSizer->Add(colorButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* selectButton = new wxButton(this, wxID_ANY, "Select Shape");
    selectButton->Bind(wxEVT_BUTTON, &MainFrame::OnSelectShape, this);
    buttonSizer->Add(selectButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* drawButton = new wxButton(this, wxID_ANY, "Start Drawing");
    drawButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartDrawing, this);
    buttonSizer->Add(drawButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* triangleButton = new wxButton(this, wxID_ANY, "Draw Triangle");
    triangleButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartDrawingTriangle, this);
    buttonSizer->Add(triangleButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* squareButton = new wxButton(this, wxID_ANY, "Draw Square");
    squareButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartDrawingSquare, this);
    buttonSizer->Add(squareButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* straightLineButton = new wxButton(this, wxID_ANY, "Draw Straight Line");
    straightLineButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartDrawingStraightLine, this);
    buttonSizer->Add(straightLineButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* ovalButton = new wxButton(this, wxID_ANY, "Draw Oval");
    ovalButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartDrawingOval, this);
    buttonSizer->Add(ovalButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* textButton = new wxButton(this, wxID_ANY, "Add Text");
    textButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartWritingText, this);
    buttonSizer->Add(textButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* bezierButton = new wxButton(this, wxID_ANY, "Draw Bézier Curve");
    bezierButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartDrawingBezier, this);
    buttonSizer->Add(bezierButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* editBezierButton = new wxButton(this, wxID_ANY, "Edit Bézier Curve");
    editBezierButton->Bind(wxEVT_BUTTON, &MainFrame::OnStartEditingBezier, this);
    buttonSizer->Add(editBezierButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* showCubeButton = new wxButton(this, wxID_ANY, "Show RGB Cube");
    showCubeButton->Bind(wxEVT_BUTTON, &MainFrame::OnShowRGBCube, this);
    buttonSizer->Add(showCubeButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* saveShapesButton = new wxButton(this, wxID_ANY, "Save");
    saveShapesButton->Bind(wxEVT_BUTTON, &MainFrame::OnSave, this);
    buttonSizer->Add(saveShapesButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* loadShapesButton = new wxButton(this, wxID_ANY, "Load");
    loadShapesButton->Bind(wxEVT_BUTTON, &MainFrame::OnLoad, this);
    buttonSizer->Add(loadShapesButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton* saveButton = new wxButton(this, wxID_ANY, "Save as PNG");
    saveButton->Bind(wxEVT_BUTTON, &MainFrame::OnSaveAsPNG, this);
    buttonSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 5);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    mainSizer->Add(canvas, 1, wxEXPAND);

    SetSizer(mainSizer);
}

void MainFrame::OnSaveAsPNG(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, _("Save PNG file"), "", "",
        "PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxSize canvasSize = canvas->GetSize();
    wxBitmap bitmap(canvasSize);

    wxMemoryDC memoryDC(bitmap);
    memoryDC.SetBackground(*wxWHITE_BRUSH);
    memoryDC.Clear();

    canvas->DrawToDC(memoryDC);

    bitmap.SaveFile(saveFileDialog.GetPath(), wxBITMAP_TYPE_PNG);
}

void MainFrame::OnStartDrawing(wxCommandEvent& event) {
    canvas->StartDrawing();
}

void MainFrame::OnStartDrawingTriangle(wxCommandEvent& event) {
    canvas->StartDrawingTriangle();
}

void MainFrame::OnStartDrawingSquare(wxCommandEvent& event) {
    canvas->StartDrawingSquare();
}

void MainFrame::OnStartDrawingStraightLine(wxCommandEvent& event) {
    canvas->StartDrawingStraightLine();
}
void MainFrame::OnStartEditingBezier(wxCommandEvent& event) {
    canvas->StartEditingBezier();
}


void MainFrame::OnStartDrawingBezier(wxCommandEvent& event) {
    canvas->StartDrawingBezier();
}

void MainFrame::OnStartDrawingOval(wxCommandEvent& event) {
    canvas->StartDrawingOval();
}

void MainFrame::OnStartWritingText(wxCommandEvent& event) {
    canvas->StartWritingText();
}

void MainFrame::OnChooseColor(wxCommandEvent& event) {
    ColorPickerDialog colorDialog(this);
    if (colorDialog.ShowModal() == wxID_OK) {
        wxColour selectedColor = colorDialog.GetSelectedColor();
        canvas->SetCurrentColor(selectedColor);
    }
}
void MainFrame::OnShowRGBCube(wxCommandEvent& event) {
    wxFrame* cubeFrame = new wxFrame(this, wxID_ANY, "RGB Color Cube", wxDefaultPosition, wxSize(600, 600));
    RGBCube* cubeCanvas = new RGBCube(cubeFrame);
    cubeFrame->Show();
}
void MainFrame::OnSelectShape(wxCommandEvent& event) {
    canvas->StartSelectingShapes();
}
void MainFrame::OnSave(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, _("Save File"), "", "",
        "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = saveFileDialog.GetPath();
    model->SaveToFile(path.ToStdString());
}

void MainFrame::OnLoad(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, _("Open File"), "", "",
        "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = openFileDialog.GetPath();
    model->LoadFromFile(path.ToStdString());
    canvas->Refresh();
}

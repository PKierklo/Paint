#include "DrawingCanvas.h"
#include <cmath>
#include "GeometryUtils.h"

DrawingCanvas::DrawingCanvas(wxFrame* parent, IDrawingModel* model)
    : wxPanel(parent), model(model)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseDown, this);
    Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
    Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
    Bind(wxEVT_RIGHT_DOWN, &DrawingCanvas::OnRightMouseDown, this);

}

void DrawingCanvas::SetCurrentColor(const wxColour& color)
{
    currentColor = color;
}

void DrawingCanvas::DrawToDC(wxDC& dc)
{
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    for (const auto& bezier : model->GetBeziers()) {
        dc.SetPen(wxPen(bezier.color, 2));
        wxPoint prevPoint = bezier.points[0];
        for (double t = 0; t <= 1; t += 0.01) {
            double u = 1 - t;
            wxPoint newPoint(
                static_cast<int>(pow(u, 3) * bezier.points[0].x +
                    3 * t * pow(u, 2) * bezier.points[1].x +
                    3 * pow(t, 2) * u * bezier.points[2].x +
                    pow(t, 3) * bezier.points[3].x),
                static_cast<int>(pow(u, 3) * bezier.points[0].y +
                    3 * t * pow(u, 2) * bezier.points[1].y +
                    3 * pow(t, 2) * u * bezier.points[2].y +
                    pow(t, 3) * bezier.points[3].y));
            dc.DrawLine(prevPoint, newPoint);
            prevPoint = newPoint;
        }

        if (isEditingBezier) {
            for (const auto& point : bezier.points) {
                dc.SetBrush(*wxRED_BRUSH);
                dc.SetPen(*wxRED_PEN);
                dc.DrawCircle(point, 5);
            }
        }
    }

    for (const auto& line : model->GetLines()) {
        dc.SetPen(wxPen(line.color, 2));
        for (size_t i = 1; i < line.points.size(); ++i) {
            dc.DrawLine(line.points[i - 1], line.points[i]);
        }
    }

    for (const auto& triangle : model->GetTriangles()) {
        vector<wxPoint> rotatedPoints;
        wxPoint center = CalculateCenter(triangle.points);
        for (const auto& point : triangle.points) {
            rotatedPoints.push_back(RotatePoint(point, center, triangle.rotationAngle * (M_PI / 180.0)));
        }
        if (rotatedPoints.size() == 3) {
            dc.SetPen(wxPen(triangle.color, 2));
            dc.DrawLine(rotatedPoints[0], rotatedPoints[1]);
            dc.DrawLine(rotatedPoints[1], rotatedPoints[2]);
            dc.DrawLine(rotatedPoints[2], rotatedPoints[0]);
        }
    }

    for (const auto& square : model->GetSquares()) {
        vector<wxPoint> rotatedPoints;
        wxPoint center = CalculateCenter(square.points);
        for (const auto& point : square.points) {
            rotatedPoints.push_back(RotatePoint(point, center, square.rotationAngle * (M_PI / 180.0)));
        }
        if (rotatedPoints.size() == 4) {
            dc.SetPen(wxPen(square.color, 2));
            dc.DrawLine(rotatedPoints[0], rotatedPoints[1]);
            dc.DrawLine(rotatedPoints[1], rotatedPoints[2]);
            dc.DrawLine(rotatedPoints[2], rotatedPoints[3]);
            dc.DrawLine(rotatedPoints[3], rotatedPoints[0]);
        }
    }

    for (const auto& line : model->GetStraightLines()) {
        dc.SetPen(wxPen(line.color, 2));
        if (line.points.size() == 2) {
            dc.DrawLine(line.points[0], line.points[1]);
        }
    }

    for (const auto& oval : model->GetOvals()) {
        wxPoint center = CalculateCenter({ oval.startPoint, oval.endPoint });
        wxPoint rotatedStart = RotatePoint(oval.startPoint, center, oval.rotationAngle * (M_PI / 180.0));
        wxPoint rotatedEnd = RotatePoint(oval.endPoint, center, oval.rotationAngle * (M_PI / 180.0));
        wxRect boundingBox(rotatedStart, rotatedEnd);
        dc.SetPen(wxPen(oval.color, 2));
        dc.DrawEllipse(boundingBox);
    }

    const auto& texts = model->GetTexts();
    for (const auto& text : texts) {
        dc.SetTextForeground(text.color); 
        dc.DrawText(text.text, text.position);
    }
}


void DrawingCanvas::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    DrawToDC(dc);


    dc.SetPen(wxPen(currentColor, 2));


    if (!selectedShape.type.IsEmpty()) {
        dc.SetPen(wxPen(*wxRED, 2, wxPENSTYLE_SOLID));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        if (selectedShape.type == "line") {
            const auto& line = model->GetLines()[selectedShape.index];
            if (!line.points.empty()) {
                wxRect boundingBox(line.points[0], line.points.back());
                dc.DrawRectangle(boundingBox);
            }
        }
        else if (selectedShape.type == "triangle") {
            const auto& triangle = model->GetTriangles()[selectedShape.index].points;
            if (triangle.size() == 3) {
                wxRect boundingBox(triangle[0], triangle[2]);
                dc.DrawRectangle(boundingBox);
            }
        }
        else if (selectedShape.type == "square") {
            const auto& square = model->GetSquares()[selectedShape.index].points;
            if (square.size() == 4) {
                wxRect boundingBox(square[0], square[2]);
                dc.DrawRectangle(boundingBox);
            }
        }
        else if (selectedShape.type == "oval") {
            const auto& oval = model->GetOvals()[selectedShape.index];
            wxRect boundingBox(oval.startPoint, oval.endPoint);
            dc.DrawRectangle(boundingBox);
        }
        else if (selectedShape.type == "text") {
            const auto& text = model->GetTexts()[selectedShape.index];
            wxRect boundingBox(text.position, wxSize(100, 20));
            dc.DrawRectangle(boundingBox);
        }

       
        DrawRotationHandle(dc);                                                                                  // Tu jest wył uchwyt obrotu
        DrawScalingHandle(dc);

    }

    const auto& currentLine = model->GetCurrentLine().points;
    for (size_t i = 1; i < currentLine.size(); ++i) {
        dc.DrawLine(currentLine[i - 1], currentLine[i]);
    }

    const auto& currentTriangle = model->GetCurrentTriangle().points;
    if (currentTriangle.size() == 3) {
        dc.DrawLine(currentTriangle[0], currentTriangle[1]);
        dc.DrawLine(currentTriangle[1], currentTriangle[2]);
        dc.DrawLine(currentTriangle[2], currentTriangle[0]);
    }

    const auto& currentSquare = model->GetCurrentSquare().points;
    if (currentSquare.size() == 4) {
        dc.DrawLine(currentSquare[0], currentSquare[1]);
        dc.DrawLine(currentSquare[1], currentSquare[2]);
        dc.DrawLine(currentSquare[2], currentSquare[3]);
        dc.DrawLine(currentSquare[3], currentSquare[0]);
    }

    const auto& currentStraightLine = model->GetCurrentStraightLine().points;
    if (currentStraightLine.size() == 2) {
        dc.DrawLine(currentStraightLine[0], currentStraightLine[1]);
    }

    const auto& currentOval = model->GetCurrentOval();
    if (currentOval.startPoint != currentOval.endPoint) {
        dc.DrawEllipse(wxRect(currentOval.startPoint, currentOval.endPoint));
    }
}

void DrawingCanvas::OnMouseDown(wxMouseEvent& event) {
    wxPoint clickPosition = event.GetPosition();

    if (isSelectingShapes) {
        if (!selectedShape.type.IsEmpty() && wxRect(scalingHandlePosition - wxPoint(5, 5), wxSize(10, 10)).Contains(clickPosition)) {
            isScaling = true;
            initialScalingMousePosition = clickPosition;
            return;
        }
        if (!selectedShape.type.IsEmpty()) {
            auto shape = model->SelectShapeAt(clickPosition);
            if (shape.type == selectedShape.type && shape.index == selectedShape.index) {
                isMovingShape = true; 
                initialClickPosition = clickPosition;
                return;
            }
        }
        if (!selectedShape.type.IsEmpty() && wxRect(rotationHandlePosition - wxPoint(5, 5), wxSize(10, 10)).Contains(clickPosition)) {

            isRotating = true;
            return;
        }

        SelectedShape shape = model->SelectShapeAt(clickPosition);
        if (!shape.type.IsEmpty()) {
            selectedShape = shape;
        }
        else {
            selectedShape = { "", 0 };
        }
        Refresh();
        return;
    }
    else if (isDrawing) {
        model->StartNewLine(event.GetPosition(), currentColor);
    }
    else if (isDrawingTriangle) {
        model->StartNewTriangle(event.GetPosition(), currentColor);
    }
    else if (isDrawingSquare) {
        model->StartNewSquare(event.GetPosition(), currentColor);
    }
    else if (isDrawingStraightLine) {
        model->StartNewStraightLine(event.GetPosition(), currentColor);
    }
    else if (isDrawingOval) {
        model->StartNewOval(event.GetPosition(), currentColor);
    }
    else if (isWritingText) {
        wxTextEntryDialog dialog(this, "Enter text:", "Text Input");
        if (dialog.ShowModal() == wxID_OK) {
            wxString text = dialog.GetValue();
            model->AddText(event.GetPosition(), text, currentColor);
        }
    }
    else if (isDrawingBezier) {
        bezierPoints.push_back(event.GetPosition());
        if (bezierPoints.size() >= 4) { 
            model->AddBezierCurve(bezierPoints, currentColor);
            bezierPoints.clear();   
        }
        Refresh();
    }
    else if (isEditingBezier) {
        const auto& beziers = model->GetBeziers();
        for (size_t i = 0; i < beziers.size(); ++i) {
            const auto& bezier = beziers[i];
            for (size_t j = 0; j < bezier.points.size(); ++j) {
                wxRect pointRect(bezier.points[j] - wxPoint(5, 5), wxSize(10, 10));
                if (pointRect.Contains(event.GetPosition())) {
                    editingBezierIndex = i;
                    editingControlPointIndex = j;
                    return;
                }
            }
        }
    }
    Refresh();
}


void DrawingCanvas::OnMouseMove(wxMouseEvent& event)
{
    if (isEditingBezier && editingBezierIndex != -1 && editingControlPointIndex != -1 && event.Dragging() && event.LeftIsDown()) {
        UpdateBezierControlPoint(event.GetPosition());
    }

    if (isScaling && event.Dragging() && event.LeftIsDown()) {
        wxPoint currentMousePosition = event.GetPosition();
        UpdateScaling(currentMousePosition);
        Refresh();
        return;
    }
    if (isRotating && event.Dragging() && event.LeftIsDown()) {
        wxPoint currentMousePosition = event.GetPosition();
        UpdateRotation(currentMousePosition);
        Refresh();
        return;
    }
    if (isMovingShape && event.Dragging() && event.LeftIsDown()) {
        wxPoint currentMousePosition = event.GetPosition();
        wxPoint delta = currentMousePosition - initialClickPosition;

        MoveSelectedShape(delta);

        initialClickPosition = currentMousePosition;
        Refresh();
        return;
    }
    if (event.Dragging() && event.LeftIsDown()) {
        if (isDrawing) {
            model->AddPoint(event.GetPosition());
        }
        else if (isDrawingTriangle) {
            model->UpdateTriangle(event.GetPosition());
        }
        else if (isDrawingSquare) {
            model->UpdateSquare(event.GetPosition());
        }
        else if (isDrawingStraightLine) {
            model->UpdateStraightLine(event.GetPosition());
        }
        else if (isDrawingOval) {
            model->UpdateOval(event.GetPosition());
        }
        Refresh();
    }
}

void DrawingCanvas::OnMouseUp(wxMouseEvent& event)
{
    if (isEditingBezier) {
        StopEditingBezier();
    }

    if (isScaling) {
        isScaling = false; 
        return;
    }
    if (isRotating) {
        isRotating = false; 
        return;
    }
    if (isMovingShape) {
        isMovingShape = false;
        return;
    }
    if (isDrawingTriangle) {
        model->UpdateTriangle(event.GetPosition());
        model->FinalizeTriangle();
    }
    else if (isDrawingSquare) {
        model->UpdateSquare(event.GetPosition());
        model->FinalizeSquare();
    }
    else if (isDrawingStraightLine) {
        model->UpdateStraightLine(event.GetPosition());
        model->FinalizeStraightLine();
    }
    else if (isDrawingOval) {
        model->UpdateOval(event.GetPosition());
        model->FinalizeOval();
    }
    Refresh();
}

void DrawingCanvas::ResetFlags() {
    isDrawing = false;
    isDrawingTriangle = false;
    isDrawingSquare = false;
    isDrawingStraightLine = false;
    isDrawingOval = false;
    isWritingText = false;
    isSelectingShapes = false;
    isRotating = false;
    isMovingShape = false;
    isDrawingBezier = false;

}

void DrawingCanvas::StartDrawing() {
    ResetFlags();
    isDrawing = true;
}

void DrawingCanvas::StartDrawingTriangle() {
    ResetFlags();
    isDrawingTriangle = true;
}

void DrawingCanvas::StartDrawingSquare() {
    ResetFlags();
    isDrawingSquare = true;
}

void DrawingCanvas::StartDrawingStraightLine() {
    ResetFlags();
    isDrawingStraightLine = true;
}

void DrawingCanvas::StartDrawingOval() {
    ResetFlags();
    isDrawingOval = true;
}

void DrawingCanvas::StartWritingText() {
    ResetFlags();
    isWritingText = true;
}
void DrawingCanvas::StartSelectingShapes() {
    ResetFlags();
    isSelectingShapes = true;
}
void DrawingCanvas::OnRightMouseDown(wxMouseEvent& event) {
    wxPoint clickPosition = event.GetPosition();

    if (!selectedShape.type.IsEmpty()) {
        ShowContextMenu(clickPosition);
    }
}
void DrawingCanvas::ShowContextMenu(const wxPoint& position) {
    wxMenu contextMenu;
    contextMenu.Append(ID_MOVE_OPTION, "Move");
    contextMenu.Append(ID_ROTATE_OPTION, "Rotate");
    contextMenu.Append(ID_SCALE_OPTION, "Scale");

    Bind(wxEVT_MENU, &DrawingCanvas::OnContextMenu, this, ID_MOVE_OPTION);
    Bind(wxEVT_MENU, &DrawingCanvas::OnContextMenu, this, ID_ROTATE_OPTION);
    Bind(wxEVT_MENU, &DrawingCanvas::OnContextMenu, this, ID_SCALE_OPTION);

    PopupMenu(&contextMenu, position);
}

void DrawingCanvas::OnContextMenu(wxCommandEvent& event) {
    switch (event.GetId()) {
    case ID_MOVE_OPTION:
        ShowMoveDialog();
        break;
    case ID_ROTATE_OPTION:
        ShowRotateDialog();
        break;
    case ID_SCALE_OPTION:
        ShowScaleDialog();
        break;
    default:
        break;
    }
}
void DrawingCanvas::ShowScaleDialog() {
    if (selectedShape.type.IsEmpty()) {
        wxMessageBox("No shape selected to scale.", "Error", wxICON_ERROR);
        return;
    }

    wxTextEntryDialog scaleDialog(this, "Enter scale factor (e.g., 1.5 for 150%, 0.5 for 50%):", "Scale Shape");
    if (scaleDialog.ShowModal() != wxID_OK) return;

    double scaleFactor;
    if (!scaleDialog.GetValue().ToDouble(&scaleFactor) || scaleFactor <= 0.0) {
        wxMessageBox("Invalid scale factor. Please enter a positive number.", "Error", wxICON_ERROR);
        return;
    }

    model->ScaleShape(selectedShape, scaleFactor);
    Refresh();
}


void DrawingCanvas::ShowRotateDialog() {
    if (selectedShape.type.IsEmpty()) {
        wxMessageBox("No shape selected to rotate.", "Error", wxICON_ERROR);
        return;
    }

    wxTextEntryDialog angleDialog(this, "Enter rotation angle (in degrees):", "Rotate Shape");
    if (angleDialog.ShowModal() != wxID_OK) return;

    long degrees;
    if (!angleDialog.GetValue().ToLong(&degrees)) {
        wxMessageBox("Invalid angle value. Please enter a numeric value.", "Error", wxICON_ERROR);
        return;
    }

    double radians = degrees * (M_PI / 180.0);                  // Radiany

    model->RotateShape(selectedShape, radians);
    Refresh();
}


void DrawingCanvas::HighlightSelectedShape(wxDC& dc) {
    if (selectedShape.type.empty()) return;

    dc.SetPen(wxPen(*wxRED, 2, wxPENSTYLE_DOT));

    if (selectedShape.type == "line") {
        const auto& line = model->GetLines()[selectedShape.index];
        for (size_t i = 1; i < line.points.size(); ++i) {
            dc.DrawLine(line.points[i - 1], line.points[i]);
        }
    }
    else if (selectedShape.type == "triangle") {
        const auto& triangle = model->GetTriangles()[selectedShape.index].points;
        if (triangle.size() == 3) {
            dc.DrawLine(triangle[0], triangle[1]);
            dc.DrawLine(triangle[1], triangle[2]);
            dc.DrawLine(triangle[2], triangle[0]);
        }
    }
    else if (selectedShape.type == "square") {
        const auto& square = model->GetSquares()[selectedShape.index].points;
        if (square.size() == 4) {
            dc.DrawLine(square[0], square[1]);
            dc.DrawLine(square[1], square[2]);
            dc.DrawLine(square[2], square[3]);
            dc.DrawLine(square[3], square[0]);
        }
    }
    else if (selectedShape.type == "oval") {
        const auto& oval = model->GetOvals()[selectedShape.index];
        dc.DrawEllipse(wxRect(oval.startPoint, oval.endPoint));
    }
    else if (selectedShape.type == "text") {
        const auto& text = model->GetTexts()[selectedShape.index];
        wxSize textSize(100, 20);
        dc.DrawRectangle(wxRect(text.position, textSize));
    }
}
void DrawingCanvas::MoveSelectedShape(const wxPoint& delta) {
    if (selectedShape.type == "line") {
        auto& line = model->GetMutableLines()[selectedShape.index];
        for (auto& point : line.points) {
            point.x += delta.x;
            point.y += delta.y;
        }
    }
    else if (selectedShape.type == "triangle") {
        auto& triangle = model->GetMutableTriangles()[selectedShape.index].points;
        for (auto& point : triangle) {
            point.x += delta.x;
            point.y += delta.y;
        }
    }
    else if (selectedShape.type == "square") {
        auto& square = model->GetMutableSquares()[selectedShape.index].points;
        for (auto& point : square) {
            point.x += delta.x;
            point.y += delta.y;
        }
    }
    else if (selectedShape.type == "oval") {
        auto& oval = model->GetMutableOvals()[selectedShape.index];
        oval.startPoint.x += delta.x;
        oval.startPoint.y += delta.y;
        oval.endPoint.x += delta.x;
        oval.endPoint.y += delta.y;
    }
    else if (selectedShape.type == "text") {
        auto& text = model->GetMutableTexts()[selectedShape.index];
        text.position.x += delta.x;
        text.position.y += delta.y;
    }
}

void DrawingCanvas::ShowMoveDialog() {
    if (selectedShape.type.IsEmpty()) {
        return; 
    }

    wxTextEntryDialog xDialog(this, "Enter horizontal shift (X):", "Move Shape");
    if (xDialog.ShowModal() != wxID_OK) return;

    long dx;
    if (!xDialog.GetValue().ToLong(&dx)) {
        wxMessageBox("Invalid value for X shift.", "Error", wxICON_ERROR);
        return;
    }

    wxTextEntryDialog yDialog(this, "Enter vertical shift (Y):", "Move Shape");
    if (yDialog.ShowModal() != wxID_OK) return;

    long dy;
    if (!yDialog.GetValue().ToLong(&dy)) {
        wxMessageBox("Invalid value for Y shift.", "Error", wxICON_ERROR);
        return;
    }

    wxPoint delta(dx, dy);

    MoveSelectedShape(delta);
    Refresh();
}
void DrawingCanvas::DrawRotationHandle(wxDC& dc) {
    wxRect boundingBox;
    if (selectedShape.type == "line") {
        const auto& line = model->GetLines()[selectedShape.index];
        boundingBox = wxRect(line.points[0], line.points.back());
    }
    else if (selectedShape.type == "triangle") {
        const auto& triangle = model->GetTriangles()[selectedShape.index].points;
        boundingBox = wxRect(triangle[0], triangle[2]);
    }
    else if (selectedShape.type == "square") {
        const auto& square = model->GetSquares()[selectedShape.index].points;
        boundingBox = wxRect(square[0], square[2]);
    }
    else if (selectedShape.type == "oval") {
        const auto& oval = model->GetOvals()[selectedShape.index];
        boundingBox = wxRect(oval.startPoint, oval.endPoint);
    }
    else if (selectedShape.type == "text") {
        const auto& text = model->GetTexts()[selectedShape.index];
        boundingBox = wxRect(text.position, wxSize(100, 20));
    }

    rotationHandlePosition = wxPoint(boundingBox.GetLeft() + boundingBox.GetWidth() / 2, boundingBox.GetTop() - 15);        //Rysowanie pozycji uchwytu obrotu

    dc.SetBrush(*wxBLUE_BRUSH);
    dc.SetPen(wxPen(*wxBLUE, 1, wxPENSTYLE_SOLID));
    dc.DrawCircle(rotationHandlePosition, 5);
}


void DrawingCanvas::UpdateRotation(const wxPoint& mousePosition) {
    wxPoint center = rotationHandlePosition;                                // Środek obrotu

    double dx = mousePosition.x - center.x;
    double dy = mousePosition.y - center.y;

    double angle = atan2(dy, dx);         //Fajna funkcja do obliczania kąta w radianach pomiędzy osiami współrzędnych a wektorem wskazywanym przez współrzędne

    if (!selectedShape.type.IsEmpty()) {
        model->RotateShape(selectedShape, angle - currentRotationAngle);
        currentRotationAngle = angle;
    }
}
void DrawingCanvas::DrawScalingHandle(wxDC& dc) {
    wxRect boundingBox;
    if (selectedShape.type == "line") {
        const auto& line = model->GetLines()[selectedShape.index];
        boundingBox = wxRect(line.points[0], line.points.back());
    }
    else if (selectedShape.type == "triangle") {
        const auto& triangle = model->GetTriangles()[selectedShape.index].points;
        boundingBox = wxRect(triangle[0], triangle[2]);
    }
    else if (selectedShape.type == "square") {
        const auto& square = model->GetSquares()[selectedShape.index].points;
        boundingBox = wxRect(square[0], square[2]);
    }
    else if (selectedShape.type == "oval") {
        const auto& oval = model->GetOvals()[selectedShape.index];
        boundingBox = wxRect(oval.startPoint, oval.endPoint);
    }
    else if (selectedShape.type == "text") {
        const auto& text = model->GetTexts()[selectedShape.index];
        boundingBox = wxRect(text.position, wxSize(100, 20));
    }

    scalingHandlePosition = wxPoint(boundingBox.GetRight() + 10, boundingBox.GetBottom() + 10);

    dc.SetBrush(*wxRED_BRUSH);
    dc.SetPen(wxPen(*wxRED, 1, wxPENSTYLE_SOLID));
    dc.DrawCircle(scalingHandlePosition, 5);
}
void DrawingCanvas::UpdateScaling(const wxPoint& mousePosition) {
    wxPoint center;
    if (selectedShape.type == "line") {
        const auto& line = model->GetLines()[selectedShape.index];
        center = CalculateCenter(line.points);
    }
    else if (selectedShape.type == "triangle") {
        const auto& triangle = model->GetTriangles()[selectedShape.index].points;
        center = CalculateCenter(triangle);
    }
    else if (selectedShape.type == "square") {
        const auto& square = model->GetSquares()[selectedShape.index].points;
        center = CalculateCenter(square);
    }
    else if (selectedShape.type == "oval") {
        const auto& oval = model->GetOvals()[selectedShape.index];
        center = CalculateCenter({ oval.startPoint, oval.endPoint });
    }
    else if (selectedShape.type == "text") {
        const auto& text = model->GetTexts()[selectedShape.index];
        center = text.position + wxPoint(50, 10); 
    }

    double initialDistance = std::hypot(initialScalingMousePosition.x - center.x,
        initialScalingMousePosition.y - center.y);
    double currentDistance = std::hypot(mousePosition.x - center.x,
        mousePosition.y - center.y);

    if (initialDistance == 0) return;

    double scaleFactor = currentDistance / initialDistance;

    model->ScaleShape(selectedShape, scaleFactor);

    initialScalingMousePosition = mousePosition;
}
void DrawingCanvas::StartDrawingBezier() {
    ResetFlags();
    isDrawingBezier = true;
    bezierPoints.clear();
}
void DrawingCanvas::StartEditingBezier() {
    ResetFlags();
    isEditingBezier = true;
    Refresh(); 
}

void DrawingCanvas::UpdateBezierControlPoint(const wxPoint& point) {
    if (editingBezierIndex != -1 && editingControlPointIndex != -1) {
        auto& bezier = model->GetMutableBeziers()[editingBezierIndex];
        bezier.points[editingControlPointIndex] = point;
        Refresh(); 
    }
}
void DrawingCanvas::StopEditingBezier() {
    isEditingBezier = false;
    editingBezierIndex = -1;
    editingControlPointIndex = -1;
    Refresh(); 
}


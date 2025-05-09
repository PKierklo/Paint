#include "DrawingModel.h"
#include "GeometryUtils.h"
#include <fstream>
#include <sstream>

void DrawingModel::AddPoint(const wxPoint& point) {
    if (currentLine.points.empty()) {
        currentLine.points.push_back(point);
    }
    currentLine.points.push_back(point);
}

void DrawingModel::StartNewLine(const wxPoint& point, const wxColour& color) {
    if (!currentLine.points.empty()) {
        lines.push_back(currentLine);
    }
    currentLine.points.clear();
    currentLine.points.push_back(point);
    currentLine.color = color;
}

void DrawingModel::FinalizeLine() {
    if (!currentLine.points.empty()) {
        lines.push_back(currentLine);
        currentLine.points.clear();
    }
}

void DrawingModel::StartNewTriangle(const wxPoint& startPoint, const wxColour& color) {
    currentTriangle.points.clear();
    currentTriangle.points.push_back(startPoint);
    currentTriangle.points.push_back(startPoint);
    currentTriangle.points.push_back(startPoint);
    currentTriangle.color = color;
}

void DrawingModel::UpdateTriangle(const wxPoint& currentPoint) {
    if (currentTriangle.points.size() == 3) {
        currentTriangle.points[1] = wxPoint(currentPoint.x, currentTriangle.points[0].y);
        currentTriangle.points[2] = currentPoint;
    }
}

void DrawingModel::FinalizeTriangle() {
    if (currentTriangle.points.size() == 3) {
        triangles.push_back(currentTriangle);
    }
    currentTriangle.points.clear();
}

void DrawingModel::StartNewSquare(const wxPoint& startPoint, const wxColour& color) {
    currentSquare.points.clear();
    currentSquare.points.push_back(startPoint);
    currentSquare.points.push_back(startPoint);
    currentSquare.points.push_back(startPoint);
    currentSquare.points.push_back(startPoint);
    currentSquare.color = color;
}

void DrawingModel::UpdateSquare(const wxPoint& currentPoint) {
    if (currentSquare.points.size() == 4) {
        currentSquare.points[1] = wxPoint(currentPoint.x, currentSquare.points[0].y);
        currentSquare.points[2] = currentPoint;
        currentSquare.points[3] = wxPoint(currentSquare.points[0].x, currentPoint.y);
    }
}

void DrawingModel::FinalizeSquare() {
    if (currentSquare.points.size() == 4) {
        squares.push_back(currentSquare);
    }
    currentSquare.points.clear();
}

void DrawingModel::StartNewStraightLine(const wxPoint& startPoint, const wxColour& color) {
    currentStraightLine.points.clear();
    currentStraightLine.points.push_back(startPoint);
    currentStraightLine.points.push_back(startPoint);
    currentStraightLine.color = color;
}

void DrawingModel::UpdateStraightLine(const wxPoint& currentPoint) {
    if (currentStraightLine.points.size() == 2) {
        currentStraightLine.points[1] = currentPoint;
    }
}

void DrawingModel::FinalizeStraightLine() {
    if (currentStraightLine.points.size() == 2) {
        straightLines.push_back(currentStraightLine);
    }
    currentStraightLine.points.clear();
}

void DrawingModel::StartNewOval(const wxPoint& startPoint, const wxColour& color) {
    currentOval.startPoint = startPoint;
    currentOval.endPoint = startPoint;
    currentOval.color = color;
}

void DrawingModel::UpdateOval(const wxPoint& currentPoint) {
    currentOval.endPoint = currentPoint;
}

void DrawingModel::FinalizeOval() {
    if (currentOval.startPoint != currentOval.endPoint) {
        ovals.push_back(currentOval);
    }
}

void DrawingModel::AddText(const wxPoint& position, const wxString& text, const wxColour& color) {
    texts.push_back({ position, text, color });
}


const vector<ColoredLine>& DrawingModel::GetLines() const {
    return lines;
}

const ColoredLine& DrawingModel::GetCurrentLine() const {
    return currentLine;
}

const vector<ColoredTriangle>& DrawingModel::GetTriangles() const {
    return triangles;
}

const ColoredTriangle& DrawingModel::GetCurrentTriangle() const {
    return currentTriangle;
}

const vector<ColoredSquare>& DrawingModel::GetSquares() const {
    return squares;
}

const ColoredSquare& DrawingModel::GetCurrentSquare() const {
    return currentSquare;
}

const vector<ColoredStraightLine>& DrawingModel::GetStraightLines() const {
    return straightLines;
}

const ColoredStraightLine& DrawingModel::GetCurrentStraightLine() const {
    return currentStraightLine;
}

const vector<ColoredOval>& DrawingModel::GetOvals() const {
    return ovals;
}

const ColoredOval& DrawingModel::GetCurrentOval() const {
    return currentOval;
}

const vector<ColoredText>& DrawingModel::GetTexts() const {
    return texts;
}

void DrawingModel::ClearPoints() {
    lines.clear();
    currentLine.points.clear();
    triangles.clear();
    currentTriangle.points.clear();
    squares.clear();
    currentSquare.points.clear();
    straightLines.clear();
    currentStraightLine.points.clear();
    ovals.clear();
    currentOval.startPoint = wxPoint();
    currentOval.endPoint = wxPoint();
    texts.clear();
}
SelectedShape DrawingModel::SelectShapeAt(const wxPoint& point) const {

    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = 1; j < lines[i].points.size(); ++j) {
            wxPoint p1 = lines[i].points[j - 1];
            wxPoint p2 = lines[i].points[j];
            wxRect lineRect(p1, p2);
            lineRect.Inflate(3);                                            // Tolerancja klikniecia przy pikselach
            if (lineRect.Contains(point)) {
                return SelectedShape{ "line", i };
            }
        }
    }

    for (size_t i = 0; i < triangles.size(); ++i) {
        const auto& t = triangles[i].points;
        if (t.size() == 3 && wxRect(t[0], t[2]).Contains(point)) {
            return SelectedShape{ "triangle", i };
        }
    }

    for (size_t i = 0; i < squares.size(); ++i) {
        const auto& s = squares[i].points;
        if (s.size() == 4 && wxRect(s[0], s[2]).Contains(point)) {
            return SelectedShape{ "square", i };
        }
    }

    for (size_t i = 0; i < ovals.size(); ++i) {
        wxRect ovalRect(ovals[i].startPoint, ovals[i].endPoint);
        if (ovalRect.Contains(point)) {
            return SelectedShape{ "oval", i };
        }
    }

    for (size_t i = 0; i < texts.size(); ++i) {
        wxRect textRect(texts[i].position, wxSize(100, 20));
        if (textRect.Contains(point)) {
            return SelectedShape{ "text", i };
        }
    }

    return SelectedShape{};
}
void DrawingModel::RotateShape(const SelectedShape& shape, double angle) {
    if (shape.type == "triangle") {
        auto& triangle = triangles[shape.index];
        triangle.rotationAngle += angle * (180.0 / M_PI);
    }
    else if (shape.type == "square") {
        auto& square = squares[shape.index];
        square.rotationAngle += angle * (180.0 / M_PI);
    }
    else if (shape.type == "oval") {
        auto& oval = ovals[shape.index];
        oval.rotationAngle += angle * (180.0 / M_PI);
    }
    else if (shape.type == "line") {
        auto& line = lines[shape.index];
        line.rotationAngle += angle * (180.0 / M_PI);
    }
}




void DrawingModel::ScaleShape(const SelectedShape& shape, double scaleFactor) {
    if (shape.type == "line") {
        auto& line = lines[shape.index];
        wxPoint center = CalculateCenter(line.points);
        for (auto& point : line.points) {
            point.x = center.x + static_cast<int>((point.x - center.x) * scaleFactor);
            point.y = center.y + static_cast<int>((point.y - center.y) * scaleFactor);
        }
    }
    else if (shape.type == "triangle") {
        auto& triangle = triangles[shape.index].points;
        wxPoint center = CalculateCenter(triangle);
        for (auto& point : triangle) {
            point.x = center.x + static_cast<int>((point.x - center.x) * scaleFactor);
            point.y = center.y + static_cast<int>((point.y - center.y) * scaleFactor);
        }
    }
    else if (shape.type == "square") {
        auto& square = squares[shape.index].points;
        wxPoint center = CalculateCenter(square);
        for (auto& point : square) {
            point.x = center.x + static_cast<int>((point.x - center.x) * scaleFactor);
            point.y = center.y + static_cast<int>((point.y - center.y) * scaleFactor);
        }
    }
    else if (shape.type == "oval") {
        auto& oval = ovals[shape.index];
        wxPoint center = CalculateCenter({ oval.startPoint, oval.endPoint });
        oval.startPoint.x = center.x + static_cast<int>((oval.startPoint.x - center.x) * scaleFactor);
        oval.startPoint.y = center.y + static_cast<int>((oval.startPoint.y - center.y) * scaleFactor);
        oval.endPoint.x = center.x + static_cast<int>((oval.endPoint.x - center.x) * scaleFactor);
        oval.endPoint.y = center.y + static_cast<int>((oval.endPoint.y - center.y) * scaleFactor);
    }
    else if (shape.type == "text") {
        auto& text = texts[shape.index];
        wxSize textSize(100, 20); 
        wxPoint center = CalculateCenter({
            text.position,
            text.position + wxPoint(textSize.GetWidth(), 0),
            text.position + wxPoint(0, textSize.GetHeight()),
            text.position + textSize
            });
        text.position.x = center.x + static_cast<int>((text.position.x - center.x) * scaleFactor);
        text.position.y = center.y + static_cast<int>((text.position.y - center.y) * scaleFactor);
    }
}
void DrawingModel::AddBezierCurve(const vector<wxPoint>& points, const wxColour& color) {
    if (points.size() >= 4) {
        beziers.push_back({ points, color });
    }
}

const vector<ColoredBezier>& DrawingModel::GetBeziers() const {
    return beziers;
}

vector<ColoredBezier>& DrawingModel::GetMutableBeziers() {
    return beziers;
}
void DrawingModel::SaveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        wxLogError("Unable to open file for saving: %s", filename);
        return;
    }

    for (const auto& line : straightLines) {
        file << "STRAIGHT_LINE " << line.color.GetAsString(wxC2S_HTML_SYNTAX).ToStdString() << " ";
        for (const auto& point : line.points) {
            file << point.x << "," << point.y << " ";
        }
        file << "\n";
    }

    for (const auto& triangle : triangles) {
        file << "TRIANGLE " << triangle.color.GetAsString(wxC2S_HTML_SYNTAX).ToStdString() << " ";
        for (const auto& point : triangle.points) {
            file << point.x << "," << point.y << " ";
        }
        file << "\n";
    }

    for (const auto& square : squares) {
        file << "SQUARE " << square.color.GetAsString(wxC2S_HTML_SYNTAX).ToStdString() << " ";
        for (const auto& point : square.points) {
            file << point.x << "," << point.y << " ";
        }
        file << "\n";
    }

    for (const auto& oval : ovals) {
        file << "OVAL " << oval.color.GetAsString(wxC2S_HTML_SYNTAX).ToStdString() << " ";
        file << oval.startPoint.x << "," << oval.startPoint.y << " ";
        file << oval.endPoint.x << "," << oval.endPoint.y << "\n";
    }

    for (const auto& bezier : beziers) {
        file << "BEZIER " << bezier.color.GetAsString(wxC2S_HTML_SYNTAX).ToStdString() << " ";
        for (const auto& point : bezier.points) {
            file << point.x << "," << point.y << " ";
        }
        file << "\n";
    }
    for (const auto& text : texts) {
        file << "TEXT " << text.color.GetAsString(wxC2S_HTML_SYNTAX).ToStdString() << " ";
        file << text.position.x << "," << text.position.y << " ";
        file << text.text.ToStdString() << "\n";
    }


    file.close();
}

void DrawingModel::LoadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        wxLogError("Unable to open file for loading: %s", filename);
        return;
    }

    ClearPoints();

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string type, colorHex;
        iss >> type >> colorHex;

        wxColour color(colorHex);

        if (type == "STRAIGHT_LINE") {
            ColoredStraightLine straightLine;
            straightLine.color = color;
            string pointStr;
            while (iss >> pointStr) {
                int x, y;
                sscanf(pointStr.c_str(), "%d,%d", &x, &y);
                straightLine.points.push_back(wxPoint(x, y));
            }
            straightLines.push_back(straightLine);
        }
        else if (type == "TRIANGLE") {
            ColoredTriangle triangle;
            triangle.color = color;
            string pointStr;
            while (iss >> pointStr) {
                int x, y;
                sscanf(pointStr.c_str(), "%d,%d", &x, &y);
                triangle.points.push_back(wxPoint(x, y));
            }
            triangles.push_back(triangle);
        }
        else if (type == "SQUARE") {
            ColoredSquare square;
            square.color = color;
            string pointStr;
            while (iss >> pointStr) {
                int x, y;
                sscanf(pointStr.c_str(), "%d,%d", &x, &y);
                square.points.push_back(wxPoint(x, y));
            }
            squares.push_back(square);
        }
        else if (type == "OVAL") {
            ColoredOval oval;
            oval.color = color;
            string point1, point2;
            iss >> point1 >> point2;

            int x1, y1, x2, y2;
            sscanf(point1.c_str(), "%d,%d", &x1, &y1);
            sscanf(point2.c_str(), "%d,%d", &x2, &y2);

            oval.startPoint = wxPoint(x1, y1);
            oval.endPoint = wxPoint(x2, y2);

            ovals.push_back(oval);
        }

    
        else if (type == "BEZIER") {
            ColoredBezier bezier;
            bezier.color = color;
            string pointStr;
            while (iss >> pointStr) {
                int x, y;
                sscanf(pointStr.c_str(), "%d,%d", &x, &y);
                bezier.points.push_back(wxPoint(x, y));
            }
            beziers.push_back(bezier);
        }
        else if (type == "TEXT") {
            ColoredText text;
            text.color = color;
            int x, y;
            char comma;
            iss >> x >> comma >> y;

            string tempText;
            getline(iss, tempText);
            text.text = tempText;
            text.text.Trim();
            text.position = wxPoint(x, y);
            texts.push_back(text);
        }

    

    }

    file.close();
}
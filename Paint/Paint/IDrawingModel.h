#ifndef IDRAWING_MODEL_H
#define IDRAWING_MODEL_H

#include <wx/wx.h>
#include <vector>
#include <utility>


using std::vector;
using std::pair;

struct ColoredText {
    wxPoint position;
    wxString text;
    wxColour color;
    double rotationAngle = 0.0;
};

struct ColoredLine {
    vector<wxPoint> points;
    wxColour color;
    double rotationAngle = 0.0;
};

struct ColoredTriangle {
    vector<wxPoint> points; 
    wxColour color;
    double rotationAngle = 0.0; 
};


struct ColoredSquare {
    vector<wxPoint> points;
    wxColour color;
    double rotationAngle = 0.0;
};

struct ColoredStraightLine {
    vector<wxPoint> points;
    wxColour color;
};

struct ColoredOval {
    wxPoint startPoint;
    wxPoint endPoint;
    wxColour color;
    double rotationAngle = 0.0;
};
struct ColoredBezier {
    std::vector<wxPoint> points; 
    wxColour color;            
};

struct SelectedShape {
    wxString type = "";
    size_t index = 0;
};

struct RotatableShape {
    std::vector<wxPoint> originalPoints;
    double currentAngle = 0.0;
};
class IDrawingModel {
public:
    virtual ~IDrawingModel() = default;

    virtual void AddPoint(const wxPoint& point) = 0;
    virtual void StartNewLine(const wxPoint& point, const wxColour& color) = 0;
    virtual void FinalizeLine() = 0;

    virtual void StartNewTriangle(const wxPoint& startPoint, const wxColour& color) = 0;
    virtual void UpdateTriangle(const wxPoint& currentPoint) = 0;
    virtual void FinalizeTriangle() = 0;

    virtual void StartNewSquare(const wxPoint& startPoint, const wxColour& color) = 0;
    virtual void UpdateSquare(const wxPoint& currentPoint) = 0;
    virtual void FinalizeSquare() = 0;

    virtual void StartNewStraightLine(const wxPoint& startPoint, const wxColour& color) = 0;
    virtual void UpdateStraightLine(const wxPoint& currentPoint) = 0;
    virtual void FinalizeStraightLine() = 0;

    virtual void StartNewOval(const wxPoint& startPoint, const wxColour& color) = 0;
    virtual void UpdateOval(const wxPoint& currentPoint) = 0;
    virtual void FinalizeOval() = 0;

    virtual void AddText(const wxPoint& position, const wxString& text, const wxColour& color) = 0;

    virtual const vector<ColoredText>& GetTexts() const = 0;
    virtual const vector<ColoredLine>& GetLines() const = 0;
    virtual const ColoredLine& GetCurrentLine() const = 0;
    virtual const vector<ColoredTriangle>& GetTriangles() const = 0;
    virtual const ColoredTriangle& GetCurrentTriangle() const = 0;
    virtual const vector<ColoredSquare>& GetSquares() const = 0;
    virtual const ColoredSquare& GetCurrentSquare() const = 0;
    virtual const vector<ColoredStraightLine>& GetStraightLines() const = 0;
    virtual const ColoredStraightLine& GetCurrentStraightLine() const = 0;
    virtual const vector<ColoredOval>& GetOvals() const = 0;
    virtual const ColoredOval& GetCurrentOval() const = 0;
    virtual vector<ColoredLine>& GetMutableLines() = 0;
    virtual vector<ColoredTriangle>& GetMutableTriangles() = 0;
    virtual vector<ColoredSquare>& GetMutableSquares() = 0;
    virtual vector<ColoredStraightLine>& GetMutableStraightLines() = 0;
    virtual vector<ColoredOval>& GetMutableOvals() = 0;
    virtual vector<ColoredText>& GetMutableTexts() = 0;
    virtual void RotateShape(const SelectedShape& shape, double angle) = 0;

    virtual void ClearPoints() = 0;
    virtual void ScaleShape(const SelectedShape& shape, double scaleFactor) = 0;

    virtual SelectedShape SelectShapeAt(const wxPoint& point) const = 0;
    virtual void AddBezierCurve(const std::vector<wxPoint>& points, const wxColour& color) = 0;

    virtual const std::vector<ColoredBezier>& GetBeziers() const = 0;

    virtual vector<ColoredBezier>& GetMutableBeziers() = 0;
    virtual void SaveToFile(const std::string& filename) const = 0;
    virtual void LoadFromFile(const std::string& filename) = 0;

};

#endif

#ifndef DRAWING_MODEL_H
#define DRAWING_MODEL_H

#include "IDrawingModel.h"
#include <map>

using namespace std;
class DrawingModel : public IDrawingModel {
public:

    SelectedShape SelectShapeAt(const wxPoint& point) const override;
    vector<ColoredLine>& GetMutableLines() { return lines; }
    vector<ColoredTriangle>& GetMutableTriangles() { return triangles; }
    vector<ColoredSquare>& GetMutableSquares() { return squares; }
    vector<ColoredStraightLine>& GetMutableStraightLines() { return straightLines; }
    vector<ColoredOval>& GetMutableOvals() { return ovals; }
    vector<ColoredText>& GetMutableTexts() { return texts; }
    void RotateShape(const SelectedShape& shape, double angle) override;


    void AddPoint(const wxPoint& point) override;
    void StartNewLine(const wxPoint& point, const wxColour& color) override;
    void FinalizeLine() override;

    void StartNewTriangle(const wxPoint& startPoint, const wxColour& color) override;
    void UpdateTriangle(const wxPoint& currentPoint) override;
    void FinalizeTriangle() override;

    void StartNewSquare(const wxPoint& startPoint, const wxColour& color) override;
    void UpdateSquare(const wxPoint& currentPoint) override;
    void FinalizeSquare() override;

    void StartNewStraightLine(const wxPoint& startPoint, const wxColour& color) override;
    void UpdateStraightLine(const wxPoint& currentPoint) override;
    void FinalizeStraightLine() override;

    void StartNewOval(const wxPoint& startPoint, const wxColour& color) override;
    void UpdateOval(const wxPoint& currentPoint) override;
    void FinalizeOval() override;

    void AddText(const wxPoint& position, const wxString& text, const wxColour& color) override;
    void ScaleShape(const SelectedShape& shape, double scaleFactor) override;

    const vector<ColoredText>& GetTexts() const override;
    const vector<ColoredLine>& GetLines() const override;
    const ColoredLine& GetCurrentLine() const override;
    const vector<ColoredTriangle>& GetTriangles() const override;
    const ColoredTriangle& GetCurrentTriangle() const override;
    const vector<ColoredSquare>& GetSquares() const override;
    const ColoredSquare& GetCurrentSquare() const override;
    const vector<ColoredStraightLine>& GetStraightLines() const override;
    const ColoredStraightLine& GetCurrentStraightLine() const override;
    const vector<ColoredOval>& GetOvals() const override;
    const ColoredOval& GetCurrentOval() const override;
    void AddBezierCurve(const std::vector<wxPoint>& points, const wxColour& color) override;
    const std::vector<ColoredBezier>& GetBeziers() const override;
    std::vector<ColoredBezier>& GetMutableBeziers() override;
    void ClearPoints() override; 
    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);


private:
    vector<ColoredLine> lines;
    ColoredLine currentLine;

    vector<ColoredTriangle> triangles;
    ColoredTriangle currentTriangle;

    vector<ColoredSquare> squares;
    ColoredSquare currentSquare;

    vector<ColoredStraightLine> straightLines;
    ColoredStraightLine currentStraightLine;

    vector<ColoredOval> ovals;
    ColoredOval currentOval;

    vector<ColoredText> texts;

    map<size_t, RotatableShape> rotatableShapes; 
    vector<ColoredBezier> beziers;
};

#endif

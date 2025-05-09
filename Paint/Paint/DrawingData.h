// DrawingData.h
#ifndef DRAWING_DATA_H
#define DRAWING_DATA_H

#include <wx/wx.h>
#include <vector>

using std::vector;

struct ColoredText {
    wxPoint position;
    wxString text;
    wxColour color;
};

struct ColoredLine {
    vector<wxPoint> points;
    wxColour color;
};

struct ColoredTriangle {
    vector<wxPoint> points;
    wxColour color;
};

struct ColoredSquare {
    vector<wxPoint> points;
    wxColour color;
};

struct ColoredStraightLine {
    vector<wxPoint> points;
    wxColour color;
};

struct ColoredOval {
    wxPoint startPoint;
    wxPoint endPoint;
    wxColour color;
};

#endif

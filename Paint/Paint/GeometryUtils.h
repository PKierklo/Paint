#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include <wx/wx.h>
#include <vector>

wxPoint CalculateCenter(const std::vector<wxPoint>& points);
wxPoint RotatePoint(const wxPoint& point, const wxPoint& center, double angle);

#endif

#include "GeometryUtils.h"
#include <cmath>

wxPoint CalculateCenter(const std::vector<wxPoint>& points) {
    int sumX = 0;
    int sumY = 0;

    for (const auto& point : points) {
        sumX += point.x;
        sumY += point.y;
    }

    int centerX = sumX / points.size();
    int centerY = sumY / points.size();

    return wxPoint(centerX, centerY);
}

wxPoint RotatePoint(const wxPoint& point, const wxPoint& center, double angle) {
    double s = std::sin(angle);
    double c = std::cos(angle);

    double x = point.x - center.x;
    double y = point.y - center.y;

    double rotatedX = x * c - y * s;
    double rotatedY = x * s + y * c;

    return wxPoint(static_cast<int>(rotatedX + center.x), static_cast<int>(rotatedY + center.y));
}

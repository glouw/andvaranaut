#include "Point.h"

#include <math.h>

static inline bool
fn(const Point point, uint8_t** party)
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && party[y + 0][x + 0] && party[y - 1][x + 0] == 0;
}

static inline bool
fe(const Point point, uint8_t** party)
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && party[y + 0][x + 0] == 0 && party[y + 0][x - 1];
}

static inline bool
fs(const Point point, uint8_t** party)
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && party[y + 0][x + 0] == 0 && party[y - 1][x + 0];
}

static inline bool
fw(const Point point, uint8_t** party)
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && party[y + 0][x + 0] && party[y + 0][x - 1] == 0;
}

static inline bool
Hor(const Point point, uint8_t** party)
{
    return fn(point, party) || fs(point, party);
}

static inline bool
Ver(const Point point, uint8_t** party)
{
    return fe(point, party) || fw(point, party);
}

Point
Point_StepNorth(const Point where, const double m, const double b)
{
    const double y = ceil(where.y - 1.0);
    const double x = (y - b) / m;
    return (Point){ x, y };
}

Point
Point_StepSouth(const Point where, const double m, const double b)
{
    const double y = floor(where.y + 1.0);
    const double x = (y - b) / m;
    return (Point){ x, y };
}

Point
Point_StepEast(const Point where, const double m, const double b)
{
    const double x = floor(where.x + 1.0);
    const double y = m * x + b;
    return (Point){ x, y };
}

Point
Point_StepWest(const Point where, const double m, const double b)
{
    const double x = ceil(where.x - 1.0);
    const double y = m * x + b;
    return (Point){ x, y };
}

Point
Point_Closest(const Point where, const Point i, const Point j)
{
    return Point_Magnitude(Point_Sub(i, where)) < Point_Magnitude(Point_Sub(j, where)) ? i : j;
}

uint8_t
Point_Tile(const Point point, uint8_t** party, const bool enclosure)
{
    const int x = point.x;
    const int y = point.y;
    if(enclosure)
    {
        if(fn(point, party)) return party[y + 0][x + 0];
        if(fe(point, party)) return party[y + 0][x - 1];
        if(fw(point, party)) return party[y + 0][x + 0];
        if(fs(point, party)) return party[y - 1][x + 0];
    }
    return party[y][x];
}


bool
Point_Collision(const Point point, uint8_t** party)
{
    const int x = point.x;
    const int y = point.y;
    return party[y][x];
}

bool
Point_Enclosure(const Point point, uint8_t** party)
{
    return Hor(point, party) || Ver(point, party);
}

int
Point_Quadrant(const double radians)
{
    const double x = cos(radians);
    const double y = sin(radians);
    if(x >= 0.0 && y >= 0.0) return 0;
    if(x <= 0.0 && y >= 0.0) return 1;
    if(x <= 0.0 && y <= 0.0) return 2;
    if(x >= 0.0 && y <= 0.0) return 3;
    return -1;
}

double
Point_Magnitude(const Point point)
{
    const double x = point.x * point.x;
    const double y = point.y * point.y;
    return sqrt(x + y);
}

double
Point_Percent(const Point point, uint8_t** party)
{
    if(fn(point, party)) return 0.0 + Point_Decimal(point.x);
    if(fe(point, party)) return 1.0 - Point_Decimal(point.y);
    if(fs(point, party)) return 1.0 - Point_Decimal(point.x);
    if(fw(point, party)) return 0.0 + Point_Decimal(point.y);
    return 0.0;
}

#include "Geom.h"

#include "Map.h"

#include <math.h>

static inline Point
sn(const Point where, const double m, const double b)
{
    const double y = ceil(where.y - 1.0);
    const double x = (y - b) / m;
    return (Point){ x, y };
}

static inline Point
se(const Point where, const double m, const double b)
{
    const double x = floor(where.x + 1.0);
    const double y = m * x + b;
    return (Point){ x, y };
}

static inline Point
ss(const Point where, const double m, const double b)
{
    const double y = floor(where.y + 1.0);
    const double x = (y - b) / m;
    return (Point){ x, y };
}

static inline Point
sw(const Point where, const double m, const double b)
{
    const double x = ceil(where.x - 1.0);
    const double y = m * x + b;
    return (Point){ x, y };
}

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
Horiztal(const Point point, uint8_t** party)
{
    return fn(point, party) || fs(point, party);
}

static inline bool
Vertical(const Point point, uint8_t** party)
{
    return fe(point, party) || fw(point, party);
}

static inline bool
Enclosure(const Point point, uint8_t** party)
{
    return Horiztal(point, party) || Vertical(point, party);
}

static inline Point
Closest(const Point where, const Point i, const Point j)
{
    return Geom_Magnitude(Geom_Sub(i, where)) < Geom_Magnitude(Geom_Sub(j, where)) ? i : j;
}

static inline int
Quadrant(const double radians)
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
Geom_Magnitude(const Point point)
{
    const double x = point.x * point.x;
    const double y = point.y * point.y;
    return sqrt(x + y);
}

double
Geom_Percent(const Point point, uint8_t** party)
{
    if(fn(point, party)) return 0.0 + Geom_Decimal(point.x);
    if(fe(point, party)) return 1.0 - Geom_Decimal(point.y);
    if(fs(point, party)) return 1.0 - Geom_Decimal(point.x);
    if(fw(point, party)) return 0.0 + Geom_Decimal(point.y);
    return 0.0;
}

static inline Point
Step(const Point where, const double m, const double b, const int q, const Map map)
{
    uint8_t** const party = map.walling;
    Point point;
    switch(q)
    {
        case 0: point = Closest(where, se(where, m, b), ss(where, m, b)); break;
        case 1: point = Closest(where, sw(where, m, b), ss(where, m, b)); break;
        case 2: point = Closest(where, sw(where, m, b), sn(where, m, b)); break;
        case 3: point = Closest(where, se(where, m, b), sn(where, m, b)); break;
    }
    if(Geom_Out(point, map)) return point;
    return Enclosure(point, party) ? point : Step(point, m, b, q, map);
}

Point
Geom_Cast(const Point where, const double radians, const Map map)
{
    const double m = tan(radians);
    const double b = where.y - m * where.x;
    const double q = Quadrant(radians);
    return Step(where, m, b, q, map);
}

bool
Geom_In(const Point point, const Map map)
{
    const bool x = point.x < (double)map.xsz && point.x > 0.0;
    const bool y = point.y < (double)map.ysz && point.y > 0.0;
    return x && y;
}

bool
Geom_Out(const Point point, const Map map)
{
    return !Geom_In(point, map);
}

bool
Geom_Collision(const Point point, uint8_t** party)
{
    const int x = point.x;
    const int y = point.y;
    return party[y][x];
}

uint8_t
Geom_Tile(const Point point, uint8_t** party, const bool enclosure)
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

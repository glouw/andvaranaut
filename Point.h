#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct point
{
    double x, y;
}
Point;

Point
Point_StepNorth(const Point where, const double m, const double b);

Point
Point_StepSouth(const Point where, const double m, const double b);

Point
Point_StepEast(const Point where, const double m, const double b);

Point
Point_StepWest(const Point where, const double m, const double b);

Point
Point_Closest(const Point where, const Point i, const Point j);

uint8_t
Point_Tile(const Point point, uint8_t** party, const bool enclosure);

bool
Point_Collision(const Point point, uint8_t** party);

bool
Point_Enclosure(const Point point, uint8_t** party);

int
Point_Quadrant(const double radians);

double
Point_Magnitude(const Point point);

double
Point_Percent(const Point point, uint8_t** party);

// ---------------------------------------------------------------------------
// Performance inlines
// ---------------------------------------------------------------------------
static inline
Point
Point_Sub(const Point i, const Point j)
{
    return (Point){ i.x - j.x, i.y - j.y };
}

static inline
Point
Point_Add(const Point i, const Point j)
{
    return (Point){ i.x + j.x, i.y + j.y };
}

static inline
Point
Point_Mul(const Point i, const double n)
{
    return (Point){ i.x * n, i.y * n };
}

static inline
double
Point_Decimal(const double d)
{
    return d - (int)d;
}

#pragma once

#include "Map.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct point
{
    double x, y;
}
Point;

double
Geom_Magnitude(const Point point);

double
Geom_Percent(const Point point, uint8_t** party);

Point
Geom_Cast(const Point where, const double radians, const Map map);

bool
Geom_Collision(const Point point, uint8_t** party);

uint8_t
Geom_Tile(const Point point, uint8_t** party, const bool enclosure);

bool
Geom_In(const Point point, const Map map);

bool
Geom_Out(const Point point, const Map map);

static inline Point
Geom_Sub(const Point i, const Point j)
{
    return (Point){ i.x - j.x, i.y - j.y };
}

static inline Point
Geom_Add(const Point i, const Point j)
{
    return (Point){ i.x + j.x, i.y + j.y };
}

static inline Point
Geom_Mul(const Point i, const double n)
{
    return (Point){ i.x * n, i.y * n };
}

static inline double
Geom_Decimal(const double d)
{
    return d - (int)d;
}

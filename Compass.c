#include "Compass.h"

#include "util.h"

#include <math.h>

// Vector needle
Compass vneedle(const Point vect)
{
    const float angle = atan2f(vect.y, vect.x);
    return (Compass) ((int) roundf(DIRS * angle / (2.0 * pi) + DIRS) % DIRS);
}

// Relational needle between two points
Compass rneedle(const Point where, const Point other)
{
    if(where.x < other.x && (int) where.y == (int) other.y) return E;
    if(where.x > other.x && (int) where.y == (int) other.y) return W;
    if(where.y > other.y && (int) where.x == (int) other.x) return S;
    return N;
}

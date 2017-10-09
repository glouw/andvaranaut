#include "Compass.h"

#include "util.h"

bool xinverted(const Compass face)
{
    return face == E || face == S;
}

Compass xneedle(const Point where, const Point other)
{
    if(where.x < other.x && (int) where.y == (int) other.y) return E;
    if(where.x > other.x && (int) where.y == (int) other.y) return W;
    if(where.y > other.y && (int) where.x == (int) other.x) return S;
    return N;
}

#include "Compass.h"

#include "util.h"

int c_is_inverted(const Compass face)
{
    return face == E || face == S;
}

Compass c_needle(const Point where, const Point other)
{
    if(where.x < other.x && (int) where.y == (int) other.y) return E;
    if(where.x > other.x && (int) where.y == (int) other.y) return W;
    if(where.y > other.y && (int) where.x == (int) other.x) return S;
    return N;
}

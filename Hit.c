#include "Hit.h"

#include "Point.h"
#include "Util.h"

static Compass needle(const Point where, const Point other)
{
    // East face
    if(where.x < other.x && (int) where.y == (int) other.y)
        return E;
    // West face
    if(where.x > other.x && (int) where.y == (int) other.y)
        return W;
    // South face
    if(where.y > other.y && (int) where.x == (int) other.x)
        return S;
    // North face
    return N;
}

static int inverted(const Compass face)
{
    return face == E || face == S;
}

static Hit collision(const Point ray, const Point direction, char** const walling)
{
    const float epsilon = 1e-3;
    const Point where = add(ray, mul(direction, epsilon));
    const Point other = sub(ray, mul(direction, epsilon));
    const float offset = dec(ray.x + ray.y);
    const Compass face = needle(where, other);
    Hit hit;
    hit.surface = tile(where, walling);
    hit.offset = inverted(face) ? 1.0 - offset : offset;
    hit.where = ray;
    hit.facing = face;
    return hit;
}

extern Hit cast(const Point where, const Point direction, char** const walling)
{
    const Point ray = cmp(where, shr(where, direction), svr(where, direction));
    const Hit hit = collision(ray, direction, walling);
    return hit.surface ? hit : cast(ray, direction, walling);
}

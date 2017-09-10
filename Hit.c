#include "Hit.h"

#include "Compass.h"
#include "util.h"

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
    const Compass face = rneedle(where, other);
    Hit hit;
    hit.surface = tile(where, walling);
    hit.offset = inverted(face) ? 1.0 - offset : offset;
    hit.where = ray;
    hit.facing = face;
    return hit;
}

Hit cast(const Point where, const Point direction, char** const walling)
{
    const Point ray = cmp(where, shr(where, direction), svr(where, direction));
    const Hit hit = collision(ray, direction, walling);
    return hit.surface ? hit : cast(ray, direction, walling);
}

#include "Hit.h"

#include "Compass.h"
#include "util.h"

static Hit collision(const Point ray, const Point direction, char** const walling)
{
    const float epsilon = 1e-3;
    const Point where = add(ray, mul(direction, epsilon));
    const Point other = sub(ray, mul(direction, epsilon));
    const float offset = xdec(ray.x + ray.y);
    const Compass face = needle(where, other);
    Hit hit;
    hit.surface = tile(where, walling);
    hit.offset = inverted(face) ? 1.0 - offset : offset;
    hit.where = ray;
    hit.facing = face;
    return hit;
}

Hit march(const Point where, const Point direction, char** const walling)
{
    const Point ray = cmp(where, shr(where, direction), svr(where, direction));
    const Hit hit = collision(ray, direction, walling);
    return hit.surface ? hit : march(ray, direction, walling);
}

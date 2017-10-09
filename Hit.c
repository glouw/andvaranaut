#include "Hit.h"

#include "Compass.h"
#include "util.h"

static Hit collision(const Point ray, const Point direction, char** const walling)
{
    const float epsilon = 1e-3;
    const Point where = xadd(ray, xmul(direction, epsilon));
    const Point other = xsub(ray, xmul(direction, epsilon));
    const float offset = xdec(ray.x + ray.y);
    const Compass face = xneedle(where, other);
    Hit hit;
    hit.surface = xtile(where, walling);
    hit.offset = xinverted(face) ? 1.0 - offset : offset;
    hit.where = ray;
    hit.facing = face;
    return hit;
}

Hit xmarch(const Point where, const Point direction, char** const walling)
{
    const Point ray = xcmp(where, xshr(where, direction), xsvr(where, direction));
    const Hit hit = collision(ray, direction, walling);
    return hit.surface ? hit : xmarch(ray, direction, walling);
}

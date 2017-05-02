#include "Hit.h"

#include "Point.h"
#include "Util.h"

static Compass needle(const Point where, const Point other)
{
    const int wx = where.x;
    const int wy = where.y;
    const int ox = other.x;
    const int oy = other.y;
    if(wx < ox && wy == oy) return E; // East facing
    if(wx > ox && wy == oy) return W; // West facing
    if(wy > oy && wx == ox) return S; // South facing
    // Default - North facing
    return N;
}

static int inverted(const Compass facing)
{
    return facing == E || facing == S;
}

static Hit collision(const Point ray, const Point direction, char** const walling)
{
    const float epsilon = 1e-3;
    const Point where = add(ray, mul(direction, epsilon));
    const Point other = sub(ray, mul(direction, epsilon));
    const float offset = dec(ray.x + ray.y);
    const Compass facing = needle(where, other);
    const Hit hit = {
        tile(where, walling),
        tile(other, walling),
        inverted(facing) ? 1.0 - offset : offset,
        ray,
        facing
    };
    return hit;
}

Hit cast(const Point where, const Point direction, char** const walling)
{
    const Point ray = cmp(where, shr(where, direction), svr(where, direction));
    const Hit hit = collision(ray, direction, walling);
    return hit.tile ? hit : cast(ray, direction, walling);
}

#include "Hit.h"

#include "Point.h"
#include "Util.h"

static Hit collision(const Point ray, const Point direction, char** const walling)
{
    const float epsilon = 1e-3;
    const Hit hit = {
        tile(add(ray, mul(direction, epsilon)), walling),
        tile(sub(ray, mul(direction, epsilon)), walling),
        dec(ray.x + ray.y),
        ray
    };
    return hit;
}

Hit cast(const Point where, const Point direction, char** const walling)
{
    const Point ray = cmp(where, shr(where, direction), svr(where, direction));
    const Hit hit = collision(ray, direction, walling);
    return hit.tile ? hit : cast(ray, direction, walling);
}

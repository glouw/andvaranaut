#include "Hit.h"

#include "util.h"

Hit collision(const Point hook, const Point direction, char** const walling)
{
    const double epsilon = 1e-3;
    return (Hit) {
        tile(add(hook, mul(direction, epsilon)), walling),
        dec(hook.x) + dec(hook.y),
        hook
    };
}

Hit cast(const Point ray, const Point direction, char** const walling)
{
    const Point hook = cmp(ray, sh(ray, direction), sv(ray, direction));
    const Hit hit = collision(hook, direction, walling);
    return hit.tile ? hit : cast(hook, direction, walling);
}

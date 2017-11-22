#include "Ray.h"

Ray xraise(Ray ray, const int yres)
{
    // Raise.
    ray.projection.top += ray.projection.height;
    ray.projection.bot += ray.projection.height;
    // Clamp top.
    ray.projection.clamped.top = ray.projection.top > yres ? yres : ray.projection.top;
    ray.projection.clamped.bot = ray.projection.bot < 0 ? 0 : ray.projection.bot;
    // Clamp bot.
    return ray;
}

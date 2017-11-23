#include "Projection.h"

#include "util.h"

Projection xproject(const int yres, const Line fov, const Point corrected)
{
    const float height = fov.a.x * yres / corrected.x;
    const float bot = yres / 2.0 - height / 2.0;
    const float top = yres / 2.0 + height / 2.0;
    const Clamped clamped = { (int) bot < 0 ? 0 : xcl(bot), (int) top > yres ? yres : xfl(top) };
    const Projection projection = { bot, top, clamped, height };
    return projection;
}

Projection xraise(const Projection p, const int yres)
{
    const float height = p.height;
    const float bot = p.top;
    const float top = p.top + height;
    const Clamped clamped = { (int) bot < 0 ? 0 : xcl(bot), (int) top > yres ? yres : xfl(top) };
    const Projection projection = { bot, top, clamped, height };
    return projection;
}

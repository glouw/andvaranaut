#include "Projection.h"

Projection project(const int yres, const Line fov, const Point corrected)
{
    const int height = focal(fov) * yres / corrected.x;
    const int bot = (yres - height) / 2;
    const int top = (yres - bot);
    const Projection projection = {
        bot, top, { bot < 0 ? 0 : bot, top > yres ? yres : top }
    };
    return projection;
}

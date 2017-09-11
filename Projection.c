#include "Projection.h"

Projection project(const int res, const Line fov, const Point corrected)
{
    const int height = focal(fov) * res / corrected.x;
    const int bot = (res - height) / 2;
    const int top = (res - bot);
    const Projection projection = {
        bot, top, { bot < 0 ? 0 : bot, top > res ? res : top }
    };
    return projection;
}

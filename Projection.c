#include "Projection.h"

Projection xproject(const int yres, const Line fov, const Point corrected)
{
    const int height = fov.a.x * yres / corrected.x;
    const int mid = yres / 2;
    const int bot = mid - height / 2;
    const int top = mid + height / 2;
    const Projection projection = {
        bot,
        top,
        mid,
        // Clamped top and bot.
        { bot < 0 ? 0 : bot, top > yres ? yres : top },
        // True integer height is recalculated here to avoid floating point error.
        top - bot
    };
    return projection;
}

#include "Wall.h"

Wall project(const int res, const Line fov, const Point corrected)
{
    const int height = focal(fov) * res / corrected.x;
    const int bot = (res - height) / 2;
    const int top = (res - bot);
    return (Wall) {
        bot, top, height, { bot < 0 ? 0 : bot, top > res ? res : top }
    };
}

Wall raise(const Wall wall, const int res)
{
    const int height = wall.height;
    const int bot = wall.bot + wall.height;
    const int top = wall.top + wall.height;
    return (Wall) {
        bot, top, height, { bot < 0 ? 0 : bot, top > res ? res : top }
    };
}

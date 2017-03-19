#include "Wall.h"

Wall project(const int res, const Line fov, const Point corrected)
{
    const int height = focal(fov) * res / corrected.x;
    const int bot = (res - height) / 2;
    const int top = (res - bot);
    return (Wall) { bot, top, height, { bot < 0 ? 0 : bot, top > res ? res : top } };
}

Wall raise(const Wall wall, const int res)
{
    Wall raised = wall;
    raised.top += raised.height;
    raised.bot += raised.height;
    raised.clamped.bot = raised.bot < 0 ? 0 : raised.bot;
    raised.clamped.top = raised.top > res ? res : raised.top;
    return raised;
}

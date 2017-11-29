#include "Projection.h"

#include "util.h"

static Clamped clamp(const int yres, const float bot, const float top)
{
    const Clamped clamp = { (int) bot < 0 ? 0 : xcl(bot), (int) top > yres ? yres : xfl(top) };
    return clamp;
}

Projection xproject(const int yres, const float focal, const float yaw, const Point corrected, const float height)
{
    const float level = 0.5;
    const float size = focal * yres / corrected.x;
    const float mid = yaw * yres / 2.0;
    const float bot = mid + (0.0 - height) * size;
    const float top = mid + (1.0 - height) * size;
    const Projection projection = { bot, top, clamp(yres, bot, top), size, height, yres, yaw, level };
    return projection;
}

Projection xstack(const Projection p)
{
    const float bot = p.top - 1.0;
    const float top = p.top + 1.0 + p.size;
    const Projection projection = { bot, top, clamp(p.yres, bot, top), p.size, p.height + 1.0, p.yres, p.yaw, p.level + 1.0 };
    return projection;
}

Projection xrocket(const Projection p)
{
    return xstack(xstack(xstack(xstack(xstack(p)))));
}

float xccast(const Projection p, const int x)
{
    return (2.0 * p.level - p.height) * p.size / ((x + 1) - p.yaw * (p.yres / 2));
}

float xfcast(const Projection p, const int x)
{
    return p.height * p.size / (p.yaw * (p.yres / 2) - (x - 1));
}

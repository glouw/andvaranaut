#include "Projection.h"

#include "util.h"

// Clamps the floating point top and bottom of the wall to the correct integer values.
// The clamped bot will always be greater than the true bot. Likewise, the clamped top
// will always be less than the true top. This will prevent the scanline wall renderer
// from experiencing rounding errors and accessing invalid texure indices.
// The clamped value will not exceed the y-resolution of the screen. This acts as a cull,
// and will precent accidental offscreen renderering.
static Clamped clamp(const int yres, const float bot, const float top)
{
    const Clamped clamp = { (int) bot < 0 ? 0 : xcl(bot), (int) top > yres ? yres : xfl(top) };
    return clamp;
}

Projection xproject(const int yres, const float focal, const float yaw, const Point corrected)
{
    const float size = focal * yres / corrected.x;
    const float half = size / 2.0;
    const float mid = yaw * yres / 2.0;
    const float bot = mid - half;
    const float top = mid + half;
    const float height = 0.5;
    const Projection projection = { bot, top, clamp(yres, bot, top), size, height, yres, yaw };
    return projection;
}

Projection xstack(const Projection p)
{
    // Must subtract one as top and bot are noninclusive to the raise.
    const float bot = p.top - 1.0;
    const float top = p.top + 1.0 + p.size;
    const float height = p.height + 1.0;
    const Projection projection = { bot, top, clamp(p.yres, bot, top), p.size, height, p.yres, p.yaw };
    return projection;
}

Projection xrocket(const Projection p)
{
    return xstack(xstack(xstack(xstack(xstack(p))))); // lol
}

float xccast(const Projection p, const int x)
{
    return p.height * p.size / ((x + 1) - p.yaw * (p.yres / 2));
}

float xfcast(const Projection p, const int x)
{
    return p.height * p.size / (p.yaw * (p.yres / 2) - (x - 1));
}

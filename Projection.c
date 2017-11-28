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
    const float size = focal * yres / corrected.x, half = size / 2.0;
    const float mid = yaw * yres / 2.0;
    const float bot = mid - half;
    const float top = mid + half;
    const float height = 0.5;
    const Projection projection = { bot, top, clamp(yres, bot, top), size, height };
    return projection;
}

Projection xstack(const Projection p, const int yres)
{
    // Must subtract one as top and bot are noninclusive to the raise.
    const float bot = p.top - 1.0;
    const float top = p.top + 1.0 + p.size ;
    const float height = p.height + 1.0;
    const Projection projection = { bot, top, clamp(yres, bot, top), p.size , height };
    return projection;
}

float xccast(const Projection proj, const int x, const float yaw, const int yres)
{
    return proj.height * proj.size / ((x + 1) - yaw * (yres / 2));
}

float xfcast(const Projection proj, const int x, const float yaw, const int yres)
{
    return proj.height * proj.size / (yaw * (yres / 2) - (x - 1));
}

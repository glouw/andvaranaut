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
    const float height = focal * yres / corrected.x, half = height / 2.0;
    const float mid = yaw * yres / 2.0;
    const float bot = mid - half;
    const float top = mid + half;
    const Projection projection = { bot, top, mid, clamp(yres, bot, top), height };
    return projection;
}

Projection xstack(const Projection p, const int yres)
{
    const float height = p.height;
    // Must subtract one as top and bot are noninclusive to the raise.
    // The middle section is not raised because raised walls will never have a ceiling;
    // A raised wall will expose the open sky which will terminate on the horizon.
    const float bot = p.top - 1.0;
    const float top = p.top - 1.0 + height;
    const Projection projection = { bot, top, p.mid, clamp(yres, bot, top), height };
    return projection;
}

Projection xoverlay(Projection a, const Projection b)
{
    if(a.clamped.top > b.clamped.bot) a.clamped.top = b.clamped.bot;
    return a;
}

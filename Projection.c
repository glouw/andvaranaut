#include "Projection.h"

#include <math.h>

#include "util.h"

static Clamped clamp(const int yres, const float bot, const float top)
{
    const Clamped clamp = {
        (int) bot < 0 ? 0 : xcl(bot), (int) top > yres ? yres : xfl(top)
    };
    return clamp;
}

Projection xproject(const int yres, const int xres, const float focal, const float yaw, const Point corrected, const float height)
{
    const float a = 0.0f; // Base
    const float b = 0.0f; // Head
    // The corrected x distance must be clamped to a value small enough otherwise size will
    // exceed the limitations of single precision floating point. The clamp value is arbitrary.
    const float size = (focal * xres / 2.0f) / (corrected.x < 1e-5f ? 1e-5f : corrected.x);
    const float mid = yaw * yres / 2.0f;
    const float bot = mid + (0.0f - height) * size;
    const float top = mid + (1.0f - height) * size;
    const Projection projection = {
        bot, top, clamp(yres, bot, top), size, height, yres, mid, a, b
    };
    return projection;
}

Projection xsheer(const Projection p, const float a, const float b)
{
    const float bot = p.bot + p.size * a - (b >= a ? 1.0f : 0.0f); // Pretty ridiculous imo, but there is no other way
    const float top = p.top + p.size * b + (b >= a ? 0.0f : 2.0f); // to fix the missing pixels between stacks.
    const Projection projection = {
        bot, top, clamp(p.yres, bot, top), p.size, p.height, p.yres, p.mid, fabsf(a), fabsf(b)
    };
    return projection;
}

float xccast(const Projection p, const int x)
{
    return (1.0f - p.height + p.b) * p.size / (x + 1 - p.mid);
}

float xfcast(const Projection p, const int x)
{
    return (0.0f - p.height - p.a) * p.size / (x + 0 - p.mid);
}

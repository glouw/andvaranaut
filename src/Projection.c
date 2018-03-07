#include "Projection.h"

#include "Clamped.h"

#include <math.h>

Projection xproject(const int yres, const int xres, const float focal, const float yaw, const Point corrected, const float height)
{
    const Sheer sheer = { 0.0f, 0.0f };
    // The corrected x distance must be clamped to a value small enough otherwise size will
    // exceed the limitations of single precision floating point. The clamp value is arbitrary.
    const float size = (focal * xres / 2.0f) / (corrected.x < 1e-5f ? 1e-5f : corrected.x);
    const float mid = yaw * yres / 2.0f;
    const float bot = mid + (0.0f - height) * size;
    const float top = mid + (1.0f - height) * size;
    const Projection projection = {
        bot, top, xclamp(yres, bot, top), size, height, yres, mid, sheer
    };
    return projection;
}

Projection xsheer(Projection p, const Sheer s)
{
    const Sheer sheer = { fabsf(s.a), fabsf(s.b) };
    const int up = s.b >= s.a;
    p.bot += p.size * s.a - (up ? 1.0f : 0.0f); // Pretty ridiculous imo, but there is no other way
    p.top += p.size * s.b + (up ? 0.0f : 2.0f); // to fix the missing pixels between stacks.
    p.clamped = xclamp(p.yres, p.bot, p.top);
    p.sheer = sheer;
    return p;
}

float xccast(const Projection p, const int x)
{
    return (1.0f - p.height + p.sheer.b) * p.size / (x + 1 - p.mid);
}

float xfcast(const Projection p, const int x)
{
    return (0.0f - p.height - p.sheer.a) * p.size / (x + 0 - p.mid);
}

#include "Projection.h"

#include "Clamped.h"

#include <math.h>

Projection p_project(const int yres, const int xres, const float focal, const float pitch, const Point corrected, const float height)
{
    const float min = 1e-5;
    const float normal = corrected.x < min ? min : corrected.x;

    static Projection zero;
    Projection p = zero;
    p.size = focal * 0.5f * xres / normal;
    p.mid = pitch * yres / 2.0f;
    p.bot = p.mid + (0.0f - height) * p.size;
    p.top = p.mid + (1.0f - height) * p.size;
    p.clamped = c_clamp(yres, p.bot, p.top);
    p.height = height;
    p.yres = yres;
    p.sheer.a = 0.0f;
    p.sheer.b = 0.0f;
    return p;
}

Projection p_sheer(Projection p, const Sheer s)
{
    //
    // Not sure where the math went wrong. Adding these pixel offsets is required.
    //

    p.bot += p.size * s.a - (s.b >= s.a  ? 1.0f : 0.0f);
    p.top += p.size * s.b + (s.b >= s.a  ? 0.0f : 2.0f);
    p.clamped = c_clamp(p.yres, p.bot, p.top);
    p.sheer.a = fabsf(s.a);
    p.sheer.b = fabsf(s.b);
    return p;
}

float p_ceil_cast(const Projection p, const int x)
{
    return (1.0f - p.height + p.sheer.b) * p.size / (x + 1 - p.mid);
}

float p_flor_cast(const Projection p, const int x)
{
    return (0.0f - p.height - p.sheer.a) * p.size / (x + 0 - p.mid);
}

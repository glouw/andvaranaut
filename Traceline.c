#include "Traceline.h"

static float ccast(const Traceline traceline, const float y, const float mid)
{
    return (traceline.fov.a.x / traceline.corrected.x) * mid / ((y + 1.0) - mid);
}

static float fcast(const Traceline traceline, const float y, const float mid)
{
    return (traceline.fov.a.x / traceline.corrected.x) * mid / (mid - (y - 1.0));
}

Point xgfp(const Traceline tl, const int x, const int yres, const float yaw, const float height)
{
    return xlerp(tl.trace, height * fcast(tl, x, yaw * (yres / 2)) / yaw);
}

Point xgcp(const Traceline tl, const int x, const int yres, const float yaw, const float height)
{
    return xlerp(tl.trace, height * ccast(tl, x, yaw * (yres / 2)) / yaw);
}

#include "Traceline.h"

float xccast(const Traceline traceline, const float y, const float mid)
{
    return (traceline.fov.a.x / traceline.corrected.x) * mid / (y + 1.0 - mid);
}

float xfcast(const Traceline traceline, const float y, const float mid)
{
    return (traceline.fov.a.x / traceline.corrected.x) * mid / (mid - y - 0.0);
}

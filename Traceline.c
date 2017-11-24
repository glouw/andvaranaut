#include "Traceline.h"

float xfcast(const Traceline traceline, const float y, const float yres)
{
    return (traceline.fov.a.x / traceline.corrected.x) / (1.0 - y / (yres / 2.0));
}

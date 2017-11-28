#include "Ray.h"

Ray xoverlay(Ray a, const Ray b)
{
    if(a.proj.clamped.top > b.proj.clamped.bot)
        a.proj.clamped.top = b.proj.clamped.bot;
    return a;
}

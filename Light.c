#include "Light.h"

int illuminate(const Light light, const float distance)
{
    const int luminance = light.torch / (distance * distance);
    return luminance > 0xFF ? 0xFF : luminance;
}

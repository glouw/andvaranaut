#include "Light.h"

int illuminate(const Light light, const float distance)
{
    const int mod = light.torch / (distance * distance);
    return mod > 0xFF ? 0xFF : mod;
}

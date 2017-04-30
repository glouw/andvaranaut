#include "Light.h"

int illuminate(const Light light, const float distance)
{
    const int mod = light.torch / (distance * distance);
    return mod > 0xFF ? 0xFF : mod;
}

Light reset()
{
    const float torch = 0.0, brightness = 1000.0, dtorch = 40.0;
    const Light light = { torch, brightness, dtorch };
    return light;
}

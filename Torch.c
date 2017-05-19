#include "Torch.h"

int illuminate(const Torch torch, const float distance)
{
    const int luminance = torch.light / (distance * distance);
    return luminance > 0xFF ? 0xFF : luminance;
}

Torch fade(const Torch torch)
{
    Torch temp = torch;
    temp.light += temp.dtorch;
    return temp.light > temp.brightness ? torch : temp;
}

Torch flicker(const Torch torch)
{
    Torch temp = torch;
    temp.light -= rand() % (int) (0.25 * temp.brightness);
    temp.light = temp.light < 0 ? 0 : temp.light;
    return temp;
}

Torch reset()
{
    Torch torch;
    torch.light = 0.0;
    torch.brightness = 750.0;
    torch.dtorch = torch.brightness / 10.0;
    return torch;
}

#include "Torch.h"

#include "Util.h"

extern int illuminate(const Torch torch, const float distance)
{
    const int luminance = torch.light / distance;
    return luminance > 0xFF ? 0xFF : luminance;
}

extern Torch burn(const Torch torch)
{
    Torch temp = torch;
    temp.light += temp.dtorch;
    return temp.light > temp.brightness ? torch : temp;
}

extern Torch out()
{
    Torch torch;
    zero(torch);
    torch.light = 0.0;
    torch.brightness = 300.0;
    torch.dtorch = torch.brightness / 20.0;
    return torch;
}

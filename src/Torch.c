#include "Torch.h"

#include "util.h"

int t_illuminate(const int light, const float distance)
{
    return light / distance > 255.0f ? 255 : light / distance;
}

Torch t_burn(const Torch torch)
{
    Torch temp = torch;
    temp.light += temp.dlight;
    return temp.light > temp.brightness ? torch : temp;
}

Torch t_snuff(void)
{
    //
    // Arbitrary divisor.
    //

    static Torch zero;
    Torch torch = zero;
    torch.light = 0;
    torch.brightness = 750;
    torch.dlight = torch.brightness / 32;
    return torch;
}

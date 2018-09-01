#include "Torch.h"

#include "util.h"

Torch xztorch(void)
{
    static Torch torch;
    return torch;
}

int xilluminate(const Torch torch, const float distance)
{
    return torch.light / distance > 0xFF ? 0xFF : torch.light / distance;
}

Torch xburn(const Torch torch)
{
    Torch temp = torch;
    temp.light += temp.dlight;
    return temp.light > temp.brightness ? torch : temp;
}

Torch xsnuff(void)
{
    Torch torch = xztorch();
    torch.light = 0;
    torch.brightness = 750;
    torch.dlight = torch.brightness / 32; // Arbitrary divisor.
    return torch;
}

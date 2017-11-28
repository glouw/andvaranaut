#pragma once

typedef struct
{
    int light;
    int brightness;
    int dlight;
}
Torch;

int xilluminate(const Torch, const float distance);

Torch xburn(const Torch);

Torch xsnuff();

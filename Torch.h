#pragma once

typedef struct
{
    float light;
    float brightness;
    float dlight;
}
Torch;

int xilluminate(const Torch torch, const float distance);

Torch xburn(const Torch torch);

Torch xout();

#pragma once

typedef struct
{
    float light;
    float brightness;
    float dlight;
}
Torch;

int xilluminate(const Torch, const float distance);

Torch xburn(const Torch);

Torch xsnuff();

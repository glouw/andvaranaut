#pragma once

typedef struct
{
    float light;
    float brightness;
    float dlight;
}
Torch;

float xilluminate(const Torch torch, const float distance);

Torch xburn(const Torch torch);

Torch xsnuff();

#pragma once

typedef struct
{
    float light;
    float brightness;
    float dlight;
}
Torch;

int illuminate(const Torch torch, const float distance);

Torch burn(const Torch torch);

Torch out();

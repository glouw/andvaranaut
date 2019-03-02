#pragma once

typedef struct
{
    int light;
    int brightness;
    int dlight;
}
Torch;

int t_illuminate(const int light, const float distance);

Torch t_burn(const Torch);

Torch t_snuff(void);

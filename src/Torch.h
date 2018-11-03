#pragma once

typedef struct
{
    int light;
    int brightness;
    int dlight;
}
Torch;

// Returns a value between 0 and 255 based on the torch light intensity and the distance of an object.
int t_illuminate(const Torch, const float distance);

Torch t_burn(const Torch);

Torch t_snuff(void);

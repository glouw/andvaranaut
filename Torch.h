#pragma once

typedef struct
{
    // Amount of torch light.
    int light;
    // Maximum amount of torch light.
    int brightness;
    // Rate of change of light.
    int dlight;
}
Torch;

// Returns a value between 0 and 255 based on the torch light
// intensity and the distance of an object.
int xilluminate(const Torch, const float distance);

// Lights up the torch if the torch is put out.
Torch xburn(const Torch);

// Puts out the torch.
Torch xsnuff();

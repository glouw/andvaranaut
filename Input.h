#pragma once

#include <stdint.h>

typedef struct
{
    // Keyboard state.
    const uint8_t* key;
    // Mouse sensitivity.
    float sx;
    float sy;
    // Mouse deltas.
    int dx;
    int dy;
    // Mouse Left, middle, right button state.
    int l;
    int m;
    int r;
}
Input;

Input xpump(Input input);

Input xready();

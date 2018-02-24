#pragma once

#include <stdint.h>

typedef struct
{
    // Keyboard state.
    const uint8_t* key;
    // Mouse sensitivity.
    float sx;
    float sy;
    // Mouse position.
    int x;
    int y;
    // Mouse deltas.
    int dx;
    int dy;
    // Mouse Left, middle, right button state.
    int l; // Left.
    int m; // Middle.
    int r; // Right.
    // Mouse button released.
    int lu; // Left.
    int mu; // Middle.
    int ru; // Right.
    // Hit 'x' on window.
    int done;
}
Input;

Input xpump(Input);

Input xready(const float sensitivity);

#pragma once

#include <stdint.h>

typedef struct
{
    // Keyboard state.
    const uint8_t* key;

    // User closed window.
    int done;

    // Mouse sensitivity.
    float sx;
    float sy;

    // Mouse position.
    int x;
    int y;

    // Mouse deltas.
    int dx;
    int dy;

    // Mouse button is pushed (Left, Middle, Right):
    int l;
    int m;
    int r;

    // Mouse button was up last frame (Left, Middle, Right).
    int lu;
    int mu;
    int ru;

    // Last states (Left, middle, right).
    int ll;
    int lm;
    int lr;
}
Input;

Input xzinput(void);

Input xpump(Input);

Input xready(const float sensitivity);

int xinvuse(const Input);

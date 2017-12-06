#include "Input.h"

#include "util.h"

#include <SDL2/SDL.h>

Input xpump(Input input)
{
    // Last mouse button states.
    static int ll; // Left.
    static int lm; // Middle.
    static int lr; // Right.
    // Keyboard events.
    SDL_PumpEvents();
    // Relative mouse state and buttons.
    const uint32_t buttons = SDL_GetRelativeMouseState(&input.dx, &input.dy);
    input.l = (buttons >> 0) & 0x1; // Left.
    input.m = (buttons >> 1) & 0x1; // Middle.
    input.r = (buttons >> 2) & 0x1; // Right.
    input.lu = (input.l == 0 && ll == 1);
    input.mu = (input.m == 0 && lm == 1);
    input.ru = (input.r == 0 && lr == 1);
    // Last mouse button state updates.
    ll = input.l;
    lm = input.m;
    lr = input.r;
    // Absolute mouse state.
    SDL_GetMouseState(&input.x, &input.y);
    return input;
}

Input xready()
{
    Input input;
    xzero(input);
    // Mouse.
    input.sx = 0.01;
    input.sy = 0.01;
    // Keyboard.
    input.key = SDL_GetKeyboardState(NULL);
    return xpump(input);
}

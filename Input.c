#include "Input.h"

#include "util.h"

#include <SDL2/SDL.h>

Input xpump(Input input)
{
    SDL_PumpEvents();
    const uint32_t buttons = SDL_GetRelativeMouseState(&input.dx, &input.dy);
    input.l = (buttons >> 0) & 0x1; // Left.
    input.m = (buttons >> 1) & 0x1; // Middle.
    input.r = (buttons >> 2) & 0x1; // Right.
    return input;
}

Input xready()
{
    Input input;
    xzero(input);
    // Mouse.
    input.sx = 0.01;
    input.sy = 0.10;
    // Keyboard.
    input.key = SDL_GetKeyboardState(NULL);
    return xpump(input);
}

#include "Input.h"

extern Input ready()
{
    Input input;
    // Mouse
    input.sensitivity = 0.01;
    input.dx = 0;
    input.dy = 0;
    // Keyboard
    input.key = SDL_GetKeyboardState(NULL);
    return input;
}

static Input keyboard(const Input input)
{
    SDL_PumpEvents();
    return input;
}

static Input mouse(const Input input)
{
    Input temp = input;
    const uint32_t buttons = SDL_GetRelativeMouseState(&temp.dx, &temp.dy);
    temp.l = (buttons >> 0) & 0x1; // Left
    temp.m = (buttons >> 1) & 0x1; // Middle
    temp.r = (buttons >> 2) & 0x1; // Right
    return temp;
}

extern Input pump(const Input input)
{
    return mouse(keyboard(input));
}

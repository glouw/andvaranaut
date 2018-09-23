#include "Input.h"

#include "util.h"

#include <SDL2/SDL.h>

Input xzinput(void)
{
    static Input input;
    return input;
}

Input xpump(Input input)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT || input.key[SDL_SCANCODE_END] || input.key[SDL_SCANCODE_ESCAPE])
        input.done = true;

    const uint32_t buttons = SDL_GetRelativeMouseState(&input.dx, &input.dy);

    input.l = (buttons >> 0) & 0x1; // Left.
    input.m = (buttons >> 1) & 0x1; // Middle.
    input.r = (buttons >> 2) & 0x1; // Right.

    input.lu = (input.l == 0 && input.ll == 1);
    input.mu = (input.m == 0 && input.lm == 1);
    input.ru = (input.r == 0 && input.lr == 1);

    input.ll = input.l;
    input.lm = input.m;
    input.lr = input.r;

    SDL_GetMouseState(&input.x, &input.y);

    return input;
}

Input xready(const float sensitivity)
{
    Input input = xzinput();

    input.sx = sensitivity;
    input.sy = sensitivity;

    input.key = SDL_GetKeyboardState(NULL);

    return xpump(input);
}

int xinvuse(const Input in)
{
    return in.key[SDL_SCANCODE_LSHIFT];
}

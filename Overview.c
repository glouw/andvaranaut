#include "Overview.h"

#include "util.h"

#include <SDL2/SDL.h>

Overview xinit()
{
    Overview ov;
    xzero(ov);
    ov.x = -1;
    ov.y = -1;
    ov.w = 32;
    ov.h = 32;
    ov.party = WALLING;
    return ov;
}

static Overview reset(Overview ov)
{
    ov.x = ov.y = -1;
    return ov;
}

Overview xupdate(Overview ov, const Input input)
{
    // Selecting either 1, 2, or 3 will change the overview party to either
    // the flooring, walling, or ceiling tiles.
    if(input.key[SDL_SCANCODE_1]) ov.party = FLORING;
    if(input.key[SDL_SCANCODE_2]) ov.party = WALLING;
    if(input.key[SDL_SCANCODE_3]) ov.party = CEILING;
    // The left mouse button will update the x and y tile position of the overview.
    if(input.l)
    {
        // Global tiles.
        ov.x = (input.x - ov.px) / ov.w;
        ov.y = (input.y - ov.py) / ov.h;
        // Relative tiles.
        const int x = input.x / ov.w;
        const int y = input.y / ov.h;
        // If the left most relative tile column was selected
        // then a tile selection is being made.
        if(x == 0) ov.selected = y, ov = reset(ov);
    }
    else ov = reset(ov);
    // The right mouse button will pan the overview x and y pixels.
    if(input.r)
    {
        ov.px += input.dx;
        ov.py += input.dy;
    }
    return ov;
}

#include "Overview.h"

#include "util.h"

#include <math.h>
#include <SDL2/SDL.h>

Overview o_init(void)
{
    static Overview zero;
    Overview ov = zero;
    ov.where.x = -1.0f;
    ov.where.y = -1.0f;
    ov.w = 32;
    ov.h = 32;
    ov.party = WALLING;
    ov.acceleration = 3.33f;
    ov.speed = 20.0f;
    return ov;
}

static Overview reset(Overview ov)
{
    ov.where.x = -1.0f;
    ov.where.y = -1.0f;
    return ov;
}

Overview o_pan(Overview ov, const Point where, const int xres, const int yres)
{
    ov.px = -ov.w * where.x + xres / 2;
    ov.py = -ov.h * where.y + yres / 2;
    return ov;
}

Overview o_update(Overview ov, const Input input, const int xres)
{
    const int textures = '~' - ' ' + 1;

    //
    // Selecting either 1, 2, or 3 will change the overview party to either the flooring, walling, or ceiling tiles.
    //

    if(input.key[SDL_SCANCODE_1]) ov.party = FLORING;
    if(input.key[SDL_SCANCODE_2]) ov.party = WALLING;
    if(input.key[SDL_SCANCODE_3]) ov.party = CEILING;

    //
    // Holding Q or E will scroll the overview wheel selectoin panel.
    //

    if(input.key[SDL_SCANCODE_Q]) ov.wheel--;
    if(input.key[SDL_SCANCODE_E]) ov.wheel++;

    //
    // Wheel clamp.
    //
    if(ov.wheel < 0)
        ov.wheel = 0;

    const int seen = xres / ov.w;

    if(textures - ov.wheel < seen)
        ov.wheel = textures - seen;

    //
    // The left mouse button will update the x and y tile position of the overview.
    //

    if(input.lu)
    {
        //
        // Overview global tiles.
        //

        ov.where.x = (input.x - ov.px) / (float) ov.w;
        ov.where.y = (input.y - ov.py) / (float) ov.h;

        //
        // Overview relative tiles.
        //

        const int x = input.x / ov.w;
        const int y = input.y / ov.h;

        //
        // Select a tile.
        //

        if(y == 0)
        {
            ov = reset(ov);
            ov.selected = ov.wheel + x;
        }
    }
    else ov = reset(ov);

    //
    // The right mouse button will pan the overview x and y pixels.
    //

    if(input.r)
    {
        ov.px += input.dx;
        ov.py += input.dy;
    }

    //
    // The arrow keys also pan with an acceleration effect.
    //

    if(input.key[SDL_SCANCODE_W]
    || input.key[SDL_SCANCODE_S]
    || input.key[SDL_SCANCODE_D]
    || input.key[SDL_SCANCODE_A])
    {
        if(input.key[SDL_SCANCODE_W]) ov.velocity.y += ov.acceleration;
        if(input.key[SDL_SCANCODE_S]) ov.velocity.y -= ov.acceleration;
        if(input.key[SDL_SCANCODE_D]) ov.velocity.x -= ov.acceleration;
        if(input.key[SDL_SCANCODE_A]) ov.velocity.x += ov.acceleration;
    }
    else ov.velocity = p_mul(ov.velocity, 1.0f - ov.acceleration / ov.speed);

    //
    // Pan top speed check.
    //

    if(p_mag(ov.velocity) > ov.speed)
        ov.velocity = p_mul(p_unit(ov.velocity), ov.speed);

    //
    // Due to integer rounding, velocities less than one must be ignored.
    //

    if(fabsf(ov.velocity.x) < 1.0f) ov.velocity.x = 0.0f;
    if(fabsf(ov.velocity.y) < 1.0f) ov.velocity.y = 0.0f;

    //
    // Add velocity to pan.
    //

    ov.px += ov.velocity.x;
    ov.py += ov.velocity.y;

    return ov;
}

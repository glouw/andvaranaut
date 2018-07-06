#include "Overview.h"

#include "util.h"

#include <math.h>
#include <SDL2/SDL.h>

Overview xzov()
{
    static Overview ov;
    return ov;
}

Overview xinit()
{
    Overview ov = xzov();
    ov.where.x = -1.0f;
    ov.where.y = -1.0f;
    ov.w = 32;
    ov.h = 32;
    ov.party = WALLING;
    ov.acceleration = 3.33f;
    ov.speed = 20.0f;
    return ov;
}

// When x and y are both -1 then then the left mouse is not pressed.
static Overview reset(Overview ov)
{
    ov.where.x = -1.0f;
    ov.where.y = -1.0f;
    return ov;
}

Overview xbackpan(Overview ov, const Point where, const int xres, const int yres)
{
    ov.px = -ov.w * where.x + xres / 2;
    ov.py = -ov.h * where.y + yres / 2;
    return ov;
}

Overview xupdate(Overview ov, const Input input, const int xres)
{
    const int textures = '~' - ' ' + 1;

    // Selecting either 1, 2, or 3 will change the overview party to either the flooring, walling, or ceiling tiles.
    if(input.key[SDL_SCANCODE_1]) ov.party = FLORING;
    if(input.key[SDL_SCANCODE_2]) ov.party = WALLING;
    if(input.key[SDL_SCANCODE_3]) ov.party = CEILING;

    // Selection wheel update.
    if(input.key[SDL_SCANCODE_Q]) ov.wheel--;
    if(input.key[SDL_SCANCODE_E]) ov.wheel++;

    // Selection wheel lower clamp.
    if(ov.wheel < 0) ov.wheel = 0;

    // Selection wheel upper clamp.
    const int seen = xres / ov.w;
    if(textures - ov.wheel < seen) ov.wheel = textures - seen;

    // The left mouse button will update the x and y tile position of the overview.
    if(input.lu)
    {
        // Overview global tiles.
        ov.where.x = (input.x - ov.px) / (float) ov.w;
        ov.where.y = (input.y - ov.py) / (float) ov.h;

        // Overview relative tiles.
        const int x = input.x / ov.w;
        const int y = input.y / ov.h;

        // If the zeroth row was chosen, then:
        if(y == 0)
        {
            // Reset the overview global tiles to -1.
            ov = reset(ov);

            // Selection update and...
            ov.selected = ov.wheel + x;
        }
    }
    // Map editing checks to see if the overview global tile selection
    // is out of map range or not before continuing. Reset here if left input is not pressed.
    else ov = reset(ov);

    // The right mouse button will pan the overview x and y pixels.
    if(input.r)
    {
        ov.px += input.dx;
        ov.py += input.dy;
    }

    // The right mouse button does not have to be used to pan.
    // The arrow keys can be used too with an acceleration effect.
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
    else ov.velocity = xmul(ov.velocity, 1.0f - ov.acceleration / ov.speed);

    // Top speed check.
    if(xmag(ov.velocity) > ov.speed) ov.velocity = xmul(xunt(ov.velocity), ov.speed);

    // Due to integer rounding, velocities less than one must be ignored.
    if(fabsf(ov.velocity.x) < 1.0f) ov.velocity.x = 0.0f;
    if(fabsf(ov.velocity.y) < 1.0f) ov.velocity.y = 0.0f;

    // Add velocity to pan.
    ov.px += ov.velocity.x;
    ov.py += ov.velocity.y;
    return ov;
}

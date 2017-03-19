#include "Frame.h"

#include "util.h"

void wrend(const Frame frame, const Wall wall, const Hit hit)
{
    const SDL_Surface* const surface = frame.gpu.surfaces.surface[hit.tile];
    const int col = surface->w * hit.offset;
    const uint32_t* const pixels = surface->pixels;
    for(int xx = wall.clamped.bot; xx < wall.clamped.top; xx++)
    {
        const int row = surface->h * (xx - wall.bot) / (wall.top - wall.bot);
        frame.display.pixels[xx + frame.yy * frame.display.width] = pixels[row + col * surface->w];
    }
}

void frend(const Frame frame, const Wall wall, const Traceline traceline, char** const floring)
{
    for(int xx = 0; xx < wall.clamped.bot; xx++)
    {
        const Point where = lerp(traceline.trace, fcast(traceline.fov, frame.res, xx) / traceline.corrected.x);
        const SDL_Surface* const surface = frame.gpu.surfaces.surface[tile(where, floring)];
        const int col = surface->w * dec(where.x);
        const int row = surface->h * dec(where.y);
        const uint32_t* const pixels = surface->pixels;
        frame.display.pixels[xx + frame.yy * frame.display.width] = pixels[row + col * surface->w];
    }
}

void crend(const Frame frame, const Wall wall, const Traceline traceline, char** const ceiling)
{
    for(int xx = wall.clamped.top; xx < frame.res; xx++)
    {
        const Point where = lerp(traceline.trace, ccast(traceline.fov, frame.res, xx) / traceline.corrected.x);
        const SDL_Surface* const surface = frame.gpu.surfaces.surface[tile(where, ceiling)];
        const int col = surface->w * dec(where.x);
        const int row = surface->h * dec(where.y);
        const uint32_t* const pixels = surface->pixels;
        frame.display.pixels[xx + frame.yy * frame.display.width] = pixels[row + col * surface->w];
    }
}

void srend(const Frame frame, const Wall wall)
{
    for(int xx = wall.clamped.top; xx < frame.res; xx++)
        frame.display.pixels[xx + frame.yy * frame.display.width] = 0x0;
}

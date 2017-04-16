#include "Scanline.h"
#include "Line.h"
#include "Point.h"
#include "Util.h"

void wrend(const Boundry boundry, const Hit hit)
{
    if(hit.tile && !hit.neighbor)
    {
        const SDL_Surface* const surface = boundry.scanline.gpu.surfaces.surface[hit.tile];
        const int row = (surface->h - 1) * hit.offset;
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        for(int x = boundry.wall.clamped.bot; x < boundry.wall.clamped.top; x++)
        {
            const int col = (surface->w - 1) * (x - boundry.wall.bot) / (boundry.wall.top - boundry.wall.bot);
            boundry.scanline.display.pixels[x + boundry.scanline.y * boundry.scanline.display.width] =
                pixels[col + row * surface->w];
        }
    }
}

void frend(const Boundry boundry, Point* const wheres, char** const floring, const Tracery tracery)
{
    for(int x = 0; x < boundry.wall.clamped.bot; x++)
    {
        const Point where = wheres[boundry.scanline.gpu.res - 1 - x] =
            lerp(tracery.traceline.trace, tracery.party[x] / tracery.traceline.corrected.x);
        const SDL_Surface* const surface = boundry.scanline.gpu.surfaces.surface[tile(where, floring)];
        const int row = (surface->h - 1) * dec(where.y);
        const int col = (surface->w - 1) * dec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        boundry.scanline.display.pixels[x + boundry.scanline.y * boundry.scanline.display.width] =
            pixels[col + row * surface->w];
    }
}

void crend(const Boundry boundry, Point* const wheres, char** const ceiling)
{
    for(int x = boundry.wall.clamped.top; x < boundry.scanline.gpu.res; x++)
    {
        const Point where = wheres[x];
        if(tile(where, ceiling))
        {
            const SDL_Surface* const surface = boundry.scanline.gpu.surfaces.surface[tile(where, ceiling)];
            const int row = (surface->h - 1) * dec(where.y);
            const int col = (surface->w - 1) * dec(where.x);
            const uint32_t* const pixels = (uint32_t*) surface->pixels;
            boundry.scanline.display.pixels[x + boundry.scanline.y * boundry.scanline.display.width] =
                pixels[col + row * surface->w];
        }
    }
}

void srend(const Boundry boundry)
{
    for(int x = boundry.wall.top; x < boundry.scanline.gpu.res; x++)
    {
        boundry.scanline.display.pixels[x + boundry.scanline.y * boundry.scanline.display.width] = 0x0;
    }
}

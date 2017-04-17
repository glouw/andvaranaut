#include "Boundary.h"
#include "Line.h"
#include "Point.h"
#include "Util.h"

void wrend(const Boundary boundary, const Hit hit)
{
    if(hit.tile && !hit.neighbor)
    {
        const SDL_Surface* const surface = boundary.scanline.gpu.surfaces.surface[hit.tile];
        const int row = (surface->h - 1) * hit.offset;
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        for(int x = boundary.wall.clamped.bot; x < boundary.wall.clamped.top; x++)
        {
            const int col = (surface->w - 1) * (x - boundary.wall.bot) / (boundary.wall.top - boundary.wall.bot);
            boundary.scanline.display.pixels[x + boundary.scanline.y * boundary.scanline.display.width] =
                pixels[col + row * surface->w];
        }
    }
}

void frend(const Boundary boundary, Point* const wheres, char** const floring, const Tracery tracery)
{
    for(int x = 0; x < boundary.wall.clamped.bot; x++)
    {
        const Point where = wheres[boundary.scanline.gpu.res - 1 - x] =
            lerp(tracery.traceline.trace, tracery.party[x] / tracery.traceline.corrected.x);
        const SDL_Surface* const surface = boundary.scanline.gpu.surfaces.surface[tile(where, floring)];
        const int row = (surface->h - 1) * dec(where.y);
        const int col = (surface->w - 1) * dec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        boundary.scanline.display.pixels[x + boundary.scanline.y * boundary.scanline.display.width] =
            pixels[col + row * surface->w];
    }
}

void crend(const Boundary boundary, Point* const wheres, char** const ceiling)
{
    for(int x = boundary.wall.clamped.top; x < boundary.scanline.gpu.res; x++)
    {
        const Point where = wheres[x];
        if(tile(where, ceiling))
        {
            const SDL_Surface* const surface = boundary.scanline.gpu.surfaces.surface[tile(where, ceiling)];
            const int row = (surface->h - 1) * dec(where.y);
            const int col = (surface->w - 1) * dec(where.x);
            const uint32_t* const pixels = (uint32_t*) surface->pixels;
            boundary.scanline.display.pixels[x + boundary.scanline.y * boundary.scanline.display.width] =
                pixels[col + row * surface->w];
        }
    }
}

void srend(const Boundary boundary, const Hero hero)
{
    for(int x = boundary.wall.clamped.top; x < boundary.scanline.gpu.res; x++)
    {
        boundary.scanline.display.pixels[x + boundary.scanline.y * boundary.scanline.display.width] = 0x0;
    }
}

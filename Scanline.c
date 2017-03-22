#include "Scanline.h"

#include "util.h"

void wrend(const Scanline scanline, const Wall wall, const Hit hit)
{
    const SDL_Surface* const surface = scanline.gpu.surfaces.surface[hit.tile];
    const int col = surface->w * hit.offset;
    const uint32_t* const pixels = surface->pixels;
    for(int x = wall.clamped.bot; x < wall.clamped.top; x++)
    {
        const int row = surface->h * (x - wall.bot) / (wall.top - wall.bot);
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[row + col * surface->w];
    }
}

void frend(const Scanline scanline, const Wall wall, const Traceline traceline, char** const floring)
{
    for(int x = 0; x < wall.clamped.bot; x++)
    {
        const Point where = lerp(traceline.trace, fcast(traceline.fov, scanline.res, x) / traceline.corrected.x);
        const SDL_Surface* const surface = scanline.gpu.surfaces.surface[tile(where, floring)];
        const int col = surface->w * dec(where.x);
        const int row = surface->h * dec(where.y);
        const uint32_t* const pixels = surface->pixels;
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[row + col * surface->w];
    }
}

void crend(const Scanline scanline, const Wall wall, const Traceline traceline, char** const ceiling)
{
    for(int x = wall.clamped.top; x < scanline.res; x++)
    {
        const Point where = lerp(traceline.trace, ccast(traceline.fov, scanline.res, x) / traceline.corrected.x);
        const SDL_Surface* const surface = scanline.gpu.surfaces.surface[tile(where, ceiling)];
        const int col = surface->w * dec(where.x);
        const int row = surface->h * dec(where.y);
        const uint32_t* const pixels = surface->pixels;
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[row + col * surface->w];
    }
}

void srend(const Scanline scanline, const Wall wall)
{
    for(int x = wall.clamped.top; x < scanline.res; x++)
        scanline.display.pixels[x + scanline.y * scanline.display.width] = 0x0;
}

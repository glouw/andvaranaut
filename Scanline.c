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

// Probably make this sky renderer a very high ceiling renderer
void srend(const Scanline scanline, const float percent)
{
    const SDL_Surface* const surface = scanline.gpu.surfaces.surface['~' - ' '];
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    const int mid = scanline.gpu.res / 2;
    const float ratio = (surface->w - 1) / (float) mid;
    const int offset = surface->h * percent;
    const int corrected = ratio * scanline.y;
    const int row = (corrected + offset) % surface->h;
    for(int x = mid; x < scanline.gpu.res; x++)
    {
        const int col = ratio * (x - mid);
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
    }
}
